// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueStyleResolver.h"

#include "Blueprint/BlueprintExtension.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "Framework/Application/SlateApplication.h"
#include "HueBlueprintExtension.h"
#include "HueSettings.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "K2Node_CallFunction.h"
#include "K2Node_CallMaterialParameterCollectionFunction.h"
#include "K2Node_Composite.h"
#include "K2Node_Copy.h"
#include "K2Node_CreateDelegate.h"
#include "K2Node_Event.h"
#include "K2Node_FormatText.h"
#include "K2Node_Knot.h"
#include "K2Node_MacroInstance.h"
#include "K2Node_MakeStruct.h"
#include "K2Node_PromotableOperator.h"
#include "K2Node_SpawnActor.h"
#include "K2Node_SpawnActorFromClass.h"
#include "K2Node_Switch.h"
#include "K2Node_Timeline.h"
#include "K2Node_Variable.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ScopedTransaction.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"

#define LOCTEXT_NAMESPACE "HueStyleResolver"

namespace HueStyleResolverPrivate
{
    static void InvalidateHueWidgets()
    {
        if (FSlateApplication::IsInitialized())
        {
            FSlateApplication::Get().InvalidateAllWidgets(false);
        }
    }

    static FHueNodeStyleOverride* GetMutableScopeStyle(
        UEdGraphNode* Node,
        EHueStyleScope Scope,
        bool bCreate)
    {
        if (!Node)
        {
            return nullptr;
        }

        if (Scope == EHueStyleScope::GlobalFunction)
        {
            const FString GlobalKey = FHueStyleResolver::GetGlobalKey(Node);
            if (GlobalKey.IsEmpty())
            {
                return nullptr;
            }

            UHueSettings* Settings = GetMutableDefault<UHueSettings>();
            if (FHueNodeStyleOverride* Existing = Settings->GlobalFunctionStyles.Find(GlobalKey))
            {
                return Existing;
            }

            return bCreate
                ? &Settings->GlobalFunctionStyles.Add(GlobalKey)
                : nullptr;
        }

        UBlueprint* Blueprint = FHueStyleResolver::GetScopeBlueprint(Node, Scope);
        if (!Blueprint)
        {
            return nullptr;
        }

        UHueBlueprintExtension* Extension = bCreate
            ? FHueStyleResolver::GetOrCreateBlueprintExtension(Blueprint)
            : FHueStyleResolver::FindBlueprintExtension(Blueprint);

        if (!Extension)
        {
            return nullptr;
        }

        if (Scope == EHueStyleScope::Instance)
        {
            if (FHueNodeStyleOverride* Existing = Extension->InstanceStyles.Find(Node->NodeGuid))
            {
                return Existing;
            }

            return bCreate
                ? &Extension->InstanceStyles.Add(Node->NodeGuid)
                : nullptr;
        }

        FHueBlueprintCategoryInfo CategoryInfo;
        if (!FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return nullptr;
        }

        if (FHueNodeStyleOverride* Existing = Extension->CategoryStyles.Find(CategoryInfo.Category))
        {
            return Existing;
        }

        return bCreate
            ? &Extension->CategoryStyles.Add(CategoryInfo.Category)
            : nullptr;
    }

    static void SaveScope(UEdGraphNode* Node, EHueStyleScope Scope)
    {
        if (Scope == EHueStyleScope::GlobalFunction)
        {
            GetMutableDefault<UHueSettings>()->SaveConfig();
            return;
        }

        if (UBlueprint* Blueprint = FHueStyleResolver::GetScopeBlueprint(Node, Scope))
        {
            Blueprint->MarkPackageDirty();
        }
    }

    static void RemoveEmptyStyle(UEdGraphNode* Node, EHueStyleScope Scope)
    {
        if (!Node)
        {
            return;
        }

        if (Scope == EHueStyleScope::GlobalFunction)
        {
            UHueSettings* Settings = GetMutableDefault<UHueSettings>();
            const FString Key = FHueStyleResolver::GetGlobalKey(Node);

            if (FHueNodeStyleOverride* Style = Settings->GlobalFunctionStyles.Find(Key))
            {
                if (Style->IsEmpty())
                {
                    Settings->GlobalFunctionStyles.Remove(Key);
                }
            }
            return;
        }

        UBlueprint* Blueprint = FHueStyleResolver::GetScopeBlueprint(Node, Scope);
        UHueBlueprintExtension* Extension = FHueStyleResolver::FindBlueprintExtension(Blueprint);
        if (!Extension)
        {
            return;
        }

        if (Scope == EHueStyleScope::Instance)
        {
            if (FHueNodeStyleOverride* Style = Extension->InstanceStyles.Find(Node->NodeGuid))
            {
                if (Style->IsEmpty())
                {
                    Extension->InstanceStyles.Remove(Node->NodeGuid);
                }
            }
            return;
        }

        FHueBlueprintCategoryInfo CategoryInfo;
        if (!FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return;
        }

        if (FHueNodeStyleOverride* Style = Extension->CategoryStyles.Find(CategoryInfo.Category))
        {
            if (Style->IsEmpty())
            {
                Extension->CategoryStyles.Remove(CategoryInfo.Category);
            }
        }
    }

    static bool ReadExplicitBlueprintCategory(
        UEdGraph* MemberGraph,
        UBlueprint* DefiningBlueprint,
        FHueBlueprintCategoryInfo& OutInfo)
    {
        if (!MemberGraph || !DefiningBlueprint)
        {
            return false;
        }

        FKismetUserDeclaredFunctionMetadata* MetaData =
            FBlueprintEditorUtils::GetGraphFunctionMetaData(MemberGraph);

        if (!MetaData)
        {
            return false;
        }

        const FText& CategoryText = MetaData->Category;
        if (CategoryText.IsEmpty()
            || CategoryText.EqualTo(UEdGraphSchema_K2::VR_DefaultCategory))
        {
            return false;
        }

        const FString Category = CategoryText.ToString().TrimStartAndEnd();
        if (Category.IsEmpty())
        {
            return false;
        }

        OutInfo.DefiningBlueprint = DefiningBlueprint;
        OutInfo.MemberGraph = MemberGraph;
        OutInfo.Category = Category;
        return true;
    }

    static bool ReadExplicitBlueprintEventCategory(
        const UK2Node_Event* EventNode,
        FHueBlueprintCategoryInfo& OutInfo)
    {
        if (!EventNode)
        {
            return false;
        }

        UFunction* Function = FFunctionFromNodeHelper::FunctionFromNode(EventNode);
        UClass* OwnerClass = Function ? Function->GetOwnerClass() : nullptr;
        UBlueprint* DefiningBlueprint =
            OwnerClass ? Cast<UBlueprint>(OwnerClass->ClassGeneratedBy) : nullptr;

        // Native engine events can have function metadata categories, but those
        // are not user-authored My Blueprint member categories.
        if (!Function || !DefiningBlueprint
            || !Function->HasMetaData(FBlueprintMetadata::MD_FunctionCategory))
        {
            return false;
        }

        const FString Category =
            Function->GetMetaData(FBlueprintMetadata::MD_FunctionCategory).TrimStartAndEnd();

        if (Category.IsEmpty()
            || FText::FromString(Category).EqualTo(UEdGraphSchema_K2::VR_DefaultCategory))
        {
            return false;
        }

        OutInfo.DefiningBlueprint = DefiningBlueprint;
        OutInfo.MemberGraph = EventNode->GetGraph();
        OutInfo.Category = Category;
        return OutInfo.MemberGraph != nullptr;
    }

    static bool GetBlueprintMemberVariableInfo(
        const UEdGraphNode* Node,
        UBlueprint*& OutDefiningBlueprint,
        FName& OutVariableName)
    {
        OutDefiningBlueprint = nullptr;
        OutVariableName = NAME_None;

        const UK2Node_Variable* VariableNode =
            Cast<UK2Node_Variable>(Node);

        if (!VariableNode || VariableNode->VariableReference.IsLocalScope())
        {
            return false;
        }

        FProperty* Property =
            VariableNode->VariableReference.ResolveMember<FProperty>(
                VariableNode->GetBlueprintClassFromNode());

        if (!Property)
        {
            return false;
        }

        UClass* OwnerClass = Property->GetOwner<UClass>();
        UBlueprint* DefiningBlueprint =
            OwnerClass ? Cast<UBlueprint>(OwnerClass->ClassGeneratedBy) : nullptr;

        if (!DefiningBlueprint)
        {
            return false;
        }

        OutDefiningBlueprint = DefiningBlueprint;
        OutVariableName = Property->GetFName();
        return OutVariableName != NAME_None;
    }

    static bool ReadExplicitBlueprintVariableCategory(
        const UEdGraphNode* Node,
        FHueBlueprintCategoryInfo& OutInfo)
    {
        UBlueprint* DefiningBlueprint = nullptr;
        FName VariableName = NAME_None;

        if (!GetBlueprintMemberVariableInfo(
            Node,
            DefiningBlueprint,
            VariableName))
        {
            return false;
        }

        const FText CategoryText =
            FBlueprintEditorUtils::GetBlueprintVariableCategory(
                DefiningBlueprint,
                VariableName,
                nullptr);

        if (CategoryText.IsEmpty()
            || CategoryText.EqualTo(UEdGraphSchema_K2::VR_DefaultCategory))
        {
            return false;
        }

        const FString Category =
            CategoryText.ToString().TrimStartAndEnd();

        if (Category.IsEmpty())
        {
            return false;
        }

        OutInfo.DefiningBlueprint = DefiningBlueprint;
        OutInfo.MemberGraph = nullptr;
        OutInfo.Category = Category;
        return true;
    }
}

bool FHueStyleResolver::IsSupportedNode(const UEdGraphNode* Node)
{
    if (!Node)
    {
        return false;
    }

    const UK2Node* K2Node = Cast<UK2Node>(Node);
    if (!K2Node)
    {
        return false;
    }

    // Keep this ordering aligned with Unreal's stock FNodeFactory. Specialized
    // presentation branches must be considered before broad interfaces such as
    // Add Pin so Hue never steals a node from a more specific native widget.
    if (K2Node->IsA<UK2Node_Composite>())
    {
        return true;
    }

    if (K2Node->DrawNodeAsVariable())
    {
        return true;
    }

    if (K2Node->IsA<UK2Node_Switch>())
    {
        return true;
    }

    if (K2Node->IsA<UK2Node_PromotableOperator>())
    {
        return true;
    }

    if (K2Node->GetClass()->ImplementsInterface(
        UK2Node_AddPinInterface::StaticClass()))
    {
        return true;
    }

    // Timeline has a dedicated Hue compatibility wrapper that reproduces the
    // one extra native presentation behavior beyond SGraphNodeK2Default: the
    // runtime timeline debug-information popup.
    if (K2Node->IsA<UK2Node_Timeline>())
    {
        return true;
    }

    // These specialized families are rendered through Hue's guarded native
    // bridge. Unreal still builds its own specialized Slate widget and Hue
    // binds only to standard visual layers, preserving the native controls.
    if (K2Node->IsA<UK2Node_CreateDelegate>()
        || K2Node->IsA<UK2Node_CallMaterialParameterCollectionFunction>())
    {
        return true;
    }

    if (K2Node->IsA<UK2Node_SpawnActor>()
        || K2Node->IsA<UK2Node_SpawnActorFromClass>())
    {
        return true;
    }

    if (K2Node->IsA<UK2Node_Event>())
    {
        return true;
    }

    if (K2Node->IsA<UK2Node_FormatText>())
    {
        return true;
    }

    // Reroute/Knot has no meaningful header/body mapping for Hue's current
    // four visual channels.
    if (K2Node->IsA<UK2Node_Knot>())
    {
        return false;
    }

    if (K2Node->IsA<UK2Node_MakeStruct>())
    {
        return true;
    }

    if (K2Node->IsA<UK2Node_Copy>())
    {
        return true;
    }

    // Compact K2 nodes are now deliberately supported. Unreal's default K2
    // compact layout uses one central visual body instead of separate title
    // and body regions. Hue maps Header Color to that compact fill, with Body
    // Color as a fallback, while Header Text Color controls the compact title.
    //
    // This covers large families such as Array/Map/Set operations,
    // Get Array Item, Last Index, subsystem reference nodes, conversions, and
    // many compact function-library calls without hardcoding each node class.
    return true;
}

bool FHueStyleResolver::TryGetColor(
    const FHueNodeStyleOverride& Style,
    EHueStyleChannel Channel,
    FLinearColor& OutColor)
{
    switch (Channel)
    {
    case EHueStyleChannel::HeaderColor:
        if (Style.bOverrideHeaderColor)
        {
            OutColor = Style.HeaderColor;
            return true;
        }
        break;

    case EHueStyleChannel::HeaderTextColor:
        if (Style.bOverrideHeaderTextColor)
        {
            OutColor = Style.HeaderTextColor;
            return true;
        }
        break;

    case EHueStyleChannel::BodyColor:
        if (Style.bOverrideBodyColor)
        {
            OutColor = Style.BodyColor;
            return true;
        }
        break;

    case EHueStyleChannel::BodyTextColor:
        if (Style.bOverrideBodyTextColor)
        {
            OutColor = Style.BodyTextColor;
            return true;
        }
        break;
    }

    return false;
}

void FHueStyleResolver::SetColor(
    FHueNodeStyleOverride& Style,
    EHueStyleChannel Channel,
    const FLinearColor& Color)
{
    switch (Channel)
    {
    case EHueStyleChannel::HeaderColor:
        Style.bOverrideHeaderColor = true;
        Style.HeaderColor = Color;
        break;

    case EHueStyleChannel::HeaderTextColor:
        Style.bOverrideHeaderTextColor = true;
        Style.HeaderTextColor = Color;
        break;

    case EHueStyleChannel::BodyColor:
        Style.bOverrideBodyColor = true;
        Style.BodyColor = Color;
        break;

    case EHueStyleChannel::BodyTextColor:
        Style.bOverrideBodyTextColor = true;
        Style.BodyTextColor = Color;
        break;
    }
}

void FHueStyleResolver::ClearColor(
    FHueNodeStyleOverride& Style,
    EHueStyleChannel Channel)
{
    switch (Channel)
    {
    case EHueStyleChannel::HeaderColor:
        Style.bOverrideHeaderColor = false;
        break;

    case EHueStyleChannel::HeaderTextColor:
        Style.bOverrideHeaderTextColor = false;
        break;

    case EHueStyleChannel::BodyColor:
        Style.bOverrideBodyColor = false;
        break;

    case EHueStyleChannel::BodyTextColor:
        Style.bOverrideBodyTextColor = false;
        break;
    }
}

FString FHueStyleResolver::GetGlobalKey(const UEdGraphNode* Node)
{
    if (!Node)
    {
        return FString();
    }

    if (const UK2Node_PromotableOperator* OperatorNode =
        Cast<UK2Node_PromotableOperator>(Node))
    {
        const FName OperationName = OperatorNode->GetOperationName();
        return OperationName != NAME_None
            ? FString(TEXT("Operator:")) + OperationName.ToString()
            : FString(TEXT("OperatorClass:")) + Node->GetClass()->GetPathName();
    }

    if (const UK2Node_CallFunction* CallFunction = Cast<UK2Node_CallFunction>(Node))
    {
        if (const UFunction* Function = CallFunction->GetTargetFunction())
        {
            // Preserve the pre-0.7 key format so existing Global Function
            // styles continue to resolve without migration.
            return Function->GetPathName();
        }
        return FString();
    }

    if (const UK2Node_Event* EventNode = Cast<UK2Node_Event>(Node))
    {
        if (const UFunction* Function = FFunctionFromNodeHelper::FunctionFromNode(EventNode))
        {
            // Use the same UFunction identity convention as function calls.
            // For a Blueprint custom event this can intentionally let the
            // declaration and calls resolve to the same project-wide style.
            return Function->GetPathName();
        }

        if (const UBlueprint* Blueprint = GetOwningBlueprint(EventNode))
        {
            return FString::Printf(
                TEXT("Event:%s:%s"),
                *Blueprint->GetPathName(),
                *EventNode->GetFunctionName().ToString());
        }

        return FString(TEXT("EventNodeClass:")) + Node->GetClass()->GetPathName();
    }

    if (const UK2Node_Variable* VariableNode = Cast<UK2Node_Variable>(Node))
    {
        UBlueprint* DefiningBlueprint = nullptr;
        FName VariableName = NAME_None;

        if (HueStyleResolverPrivate::GetBlueprintMemberVariableInfo(
            VariableNode,
            DefiningBlueprint,
            VariableName))
        {
            return FString::Printf(
                TEXT("Variable:%s:%s"),
                *DefiningBlueprint->GetPathName(),
                *VariableName.ToString());
        }
    }

    if (const UK2Node_MacroInstance* MacroNode = Cast<UK2Node_MacroInstance>(Node))
    {
        if (UEdGraph* MacroGraph = MacroNode->GetMacroGraph())
        {
            return FString(TEXT("Macro:")) + MacroGraph->GetPathName();
        }
        return FString();
    }

    return FString(TEXT("NodeClass:")) + Node->GetClass()->GetPathName();
}

FText FHueStyleResolver::GetGlobalScopeLabel(const UEdGraphNode* Node)
{
    if (Cast<UK2Node_PromotableOperator>(Node))
    {
        return LOCTEXT("GlobalOperatorLabel", "Global Operator");
    }

    if (Cast<UK2Node_CallFunction>(Node))
    {
        return LOCTEXT("GlobalFunctionLabel", "Global Function");
    }

    if (Cast<UK2Node_Event>(Node))
    {
        return LOCTEXT("GlobalEventLabel", "Global Event");
    }

    UBlueprint* VariableBlueprint = nullptr;
    FName VariableName = NAME_None;
    if (HueStyleResolverPrivate::GetBlueprintMemberVariableInfo(
        Node,
        VariableBlueprint,
        VariableName))
    {
        return LOCTEXT("GlobalVariableLabel", "Global Variable");
    }

    if (Cast<UK2Node_MacroInstance>(Node))
    {
        return LOCTEXT("GlobalMacroLabel", "Global Macro");
    }

    return LOCTEXT("GlobalNodeLabel", "Global Node");
}

FText FHueStyleResolver::GetGlobalScopeTooltip(const UEdGraphNode* Node)
{
    if (const UK2Node_PromotableOperator* OperatorNode =
        Cast<UK2Node_PromotableOperator>(Node))
    {
        const FName OperationName = OperatorNode->GetOperationName();
        return FText::Format(
            LOCTEXT(
                "GlobalOperatorTooltip",
                "Style the {0} promotable operator everywhere Hue identifies the same operator in the project, regardless of its currently promoted numeric type."),
            FText::FromName(OperationName));
    }

    if (Cast<UK2Node_CallFunction>(Node))
    {
        return LOCTEXT(
            "GlobalFunctionTooltip",
            "Style this exact function everywhere Hue identifies the same function in the project.");
    }

    if (Cast<UK2Node_Event>(Node))
    {
        return LOCTEXT(
            "GlobalEventTooltip",
            "Style this exact event signature everywhere Hue identifies the same event in the project.");
    }

    UBlueprint* VariableBlueprint = nullptr;
    FName VariableName = NAME_None;
    if (HueStyleResolverPrivate::GetBlueprintMemberVariableInfo(
        Node,
        VariableBlueprint,
        VariableName))
    {
        return LOCTEXT(
            "GlobalVariableTooltip",
            "Style this exact Blueprint member variable everywhere Hue identifies the same variable in the project. Get and Set nodes share this Global Variable style.");
    }

    if (Cast<UK2Node_MacroInstance>(Node))
    {
        return LOCTEXT(
            "GlobalMacroTooltip",
            "Style this exact macro everywhere Hue identifies the same macro in the project.");
    }

    return LOCTEXT(
        "GlobalNodeTooltip",
        "Style this node type everywhere Hue identifies the same supported node class in the project.");
}

bool FHueStyleResolver::GetBlueprintCategoryInfo(
    const UEdGraphNode* Node,
    FHueBlueprintCategoryInfo& OutInfo)
{
    OutInfo = FHueBlueprintCategoryInfo();

    if (!Node)
    {
        return false;
    }

    if (const UK2Node_CallFunction* CallFunction = Cast<UK2Node_CallFunction>(Node))
    {
        const UEdGraphNode* ResultEventNode = nullptr;
        UEdGraph* FunctionGraph = CallFunction->GetFunctionGraph(ResultEventNode);

        if (const UK2Node_Event* EventNode = Cast<UK2Node_Event>(ResultEventNode))
        {
            return HueStyleResolverPrivate::ReadExplicitBlueprintEventCategory(
                EventNode,
                OutInfo);
        }

        if (!FunctionGraph)
        {
            return false;
        }

        UBlueprint* DefiningBlueprint =
            FBlueprintEditorUtils::FindBlueprintForGraph(FunctionGraph);

        return HueStyleResolverPrivate::ReadExplicitBlueprintCategory(
            FunctionGraph,
            DefiningBlueprint,
            OutInfo);
    }

    if (const UK2Node_Event* EventNode = Cast<UK2Node_Event>(Node))
    {
        return HueStyleResolverPrivate::ReadExplicitBlueprintEventCategory(
            EventNode,
            OutInfo);
    }

    if (Cast<UK2Node_Variable>(Node))
    {
        return HueStyleResolverPrivate::ReadExplicitBlueprintVariableCategory(
            Node,
            OutInfo);
    }

    if (const UK2Node_MacroInstance* MacroNode = Cast<UK2Node_MacroInstance>(Node))
    {
        UEdGraph* MacroGraph = MacroNode->GetMacroGraph();
        UBlueprint* DefiningBlueprint = MacroNode->GetSourceBlueprint();
        if (!DefiningBlueprint && MacroGraph)
        {
            DefiningBlueprint = FBlueprintEditorUtils::FindBlueprintForGraph(MacroGraph);
        }

        return HueStyleResolverPrivate::ReadExplicitBlueprintCategory(
            MacroGraph,
            DefiningBlueprint,
            OutInfo);
    }

    return false;
}

UBlueprint* FHueStyleResolver::GetDefiningBlueprint(const UEdGraphNode* Node)
{
    if (!Node)
    {
        return nullptr;
    }

    if (const UK2Node_CallFunction* CallFunction = Cast<UK2Node_CallFunction>(Node))
    {
        const UEdGraphNode* ResultEventNode = nullptr;
        UEdGraph* FunctionGraph = CallFunction->GetFunctionGraph(ResultEventNode);

        if (const UK2Node_Event* EventNode = Cast<UK2Node_Event>(ResultEventNode))
        {
            if (UFunction* Function = FFunctionFromNodeHelper::FunctionFromNode(EventNode))
            {
                UClass* OwnerClass = Function->GetOwnerClass();
                return OwnerClass
                    ? Cast<UBlueprint>(OwnerClass->ClassGeneratedBy)
                    : nullptr;
            }
            return GetOwningBlueprint(EventNode);
        }

        return FunctionGraph
            ? FBlueprintEditorUtils::FindBlueprintForGraph(FunctionGraph)
            : nullptr;
    }

    if (const UK2Node_Event* EventNode = Cast<UK2Node_Event>(Node))
    {
        if (UFunction* Function = FFunctionFromNodeHelper::FunctionFromNode(EventNode))
        {
            UClass* OwnerClass = Function->GetOwnerClass();
            if (UBlueprint* DefiningBlueprint =
                OwnerClass ? Cast<UBlueprint>(OwnerClass->ClassGeneratedBy) : nullptr)
            {
                return DefiningBlueprint;
            }
        }

        // A custom event is authored by the Blueprint containing it even if a
        // generated signature function is temporarily unavailable.
        return GetOwningBlueprint(EventNode);
    }

    if (Cast<UK2Node_Variable>(Node))
    {
        UBlueprint* DefiningBlueprint = nullptr;
        FName VariableName = NAME_None;

        return HueStyleResolverPrivate::GetBlueprintMemberVariableInfo(
            Node,
            DefiningBlueprint,
            VariableName)
            ? DefiningBlueprint
            : nullptr;
    }

    if (const UK2Node_MacroInstance* MacroNode = Cast<UK2Node_MacroInstance>(Node))
    {
        if (UBlueprint* SourceBlueprint = MacroNode->GetSourceBlueprint())
        {
            return SourceBlueprint;
        }

        return MacroNode->GetMacroGraph()
            ? FBlueprintEditorUtils::FindBlueprintForGraph(MacroNode->GetMacroGraph())
            : nullptr;
    }

    return nullptr;
}

UBlueprint* FHueStyleResolver::GetOwningBlueprint(const UEdGraphNode* Node)
{
    return Node
        ? FBlueprintEditorUtils::FindBlueprintForNode(Node)
        : nullptr;
}

UBlueprint* FHueStyleResolver::GetScopeBlueprint(
    const UEdGraphNode* Node,
    EHueStyleScope Scope)
{
    switch (Scope)
    {
    case EHueStyleScope::Instance:
        return GetOwningBlueprint(Node);

    case EHueStyleScope::Category:
    {
        FHueBlueprintCategoryInfo Info;
        return GetBlueprintCategoryInfo(Node, Info)
            ? Info.DefiningBlueprint
            : nullptr;
    }

    case EHueStyleScope::GlobalFunction:
        return nullptr;
    }

    return nullptr;
}

UHueBlueprintExtension* FHueStyleResolver::FindBlueprintExtension(
    const UBlueprint* Blueprint)
{
    if (!Blueprint)
    {
        return nullptr;
    }

    for (const TObjectPtr<UBlueprintExtension>& Extension : Blueprint->GetExtensions())
    {
        if (UHueBlueprintExtension* HueExtension =
            Cast<UHueBlueprintExtension>(Extension.Get()))
        {
            return HueExtension;
        }
    }

    return nullptr;
}

UHueBlueprintExtension* FHueStyleResolver::GetOrCreateBlueprintExtension(
    UBlueprint* Blueprint)
{
    if (!Blueprint)
    {
        return nullptr;
    }

    if (UHueBlueprintExtension* Existing = FindBlueprintExtension(Blueprint))
    {
        return Existing;
    }

    Blueprint->Modify();

    UHueBlueprintExtension* Extension =
        NewObject<UHueBlueprintExtension>(
            Blueprint,
            NAME_None,
            RF_Transactional);

    Blueprint->AddExtension(Extension);
    Blueprint->MarkPackageDirty();
    return Extension;
}

bool FHueStyleResolver::TryGetScopeStyle(
    const UEdGraphNode* Node,
    EHueStyleScope Scope,
    const FHueNodeStyleOverride*& OutStyle)
{
    OutStyle = nullptr;

    if (!Node)
    {
        return false;
    }

    if (Scope == EHueStyleScope::GlobalFunction)
    {
        const FString Key = GetGlobalKey(Node);
        if (Key.IsEmpty())
        {
            return false;
        }

        OutStyle = GetDefault<UHueSettings>()->GlobalFunctionStyles.Find(Key);
        return OutStyle != nullptr;
    }

    const UBlueprint* Blueprint = GetScopeBlueprint(Node, Scope);
    const UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint);

    if (!Extension)
    {
        return false;
    }

    if (Scope == EHueStyleScope::Instance)
    {
        OutStyle = Extension->InstanceStyles.Find(Node->NodeGuid);
        return OutStyle != nullptr;
    }

    FHueBlueprintCategoryInfo CategoryInfo;
    if (!GetBlueprintCategoryInfo(Node, CategoryInfo))
    {
        return false;
    }

    OutStyle = Extension->CategoryStyles.Find(CategoryInfo.Category);
    return OutStyle != nullptr;
}

bool FHueStyleResolver::ResolveColor(
    const UEdGraphNode* Node,
    EHueStyleChannel Channel,
    FLinearColor& OutColor)
{
    const FHueNodeStyleOverride* Style = nullptr;

    if (TryGetScopeStyle(Node, EHueStyleScope::Instance, Style)
        && TryGetColor(*Style, Channel, OutColor))
    {
        return true;
    }

    if (TryGetScopeStyle(Node, EHueStyleScope::Category, Style)
        && TryGetColor(*Style, Channel, OutColor))
    {
        return true;
    }

    if (TryGetScopeStyle(Node, EHueStyleScope::GlobalFunction, Style)
        && TryGetColor(*Style, Channel, OutColor))
    {
        return true;
    }

    return false;
}

FLinearColor FHueStyleResolver::GetNativeColor(
    const UEdGraphNode* Node,
    EHueStyleChannel Channel)
{
    if (!Node)
    {
        return FLinearColor::White;
    }

    switch (Channel)
    {
    case EHueStyleChannel::HeaderColor:
        return Node->GetNodeTitleColor();

    case EHueStyleChannel::HeaderTextColor:
        return Node->GetNodeTitleTextColor();

    case EHueStyleChannel::BodyColor:
        // SGraphNodeK2Var draws its body brush without an additional tint.
        // White is therefore the true native multiplier for that presentation.
        if (const UK2Node* K2Node = Cast<UK2Node>(Node))
        {
            if (K2Node->DrawNodeAsVariable() || K2Node->ShouldDrawCompact())
            {
                return FLinearColor::White;
            }
        }
        return Node->GetNodeBodyTintColor();

    case EHueStyleChannel::BodyTextColor:
        return FLinearColor::White;
    }

    return FLinearColor::White;
}

FLinearColor FHueStyleResolver::GetPickerInitialColor(
    const UEdGraphNode* Node,
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    const FHueNodeStyleOverride* Style = nullptr;
    FLinearColor Color;

    if (TryGetScopeStyle(Node, Scope, Style)
        && TryGetColor(*Style, Channel, Color))
    {
        return Color;
    }

    if (Scope == EHueStyleScope::Instance)
    {
        if (TryGetScopeStyle(Node, EHueStyleScope::Category, Style)
            && TryGetColor(*Style, Channel, Color))
        {
            return Color;
        }

        if (TryGetScopeStyle(Node, EHueStyleScope::GlobalFunction, Style)
            && TryGetColor(*Style, Channel, Color))
        {
            return Color;
        }
    }
    else if (Scope == EHueStyleScope::Category)
    {
        if (TryGetScopeStyle(Node, EHueStyleScope::GlobalFunction, Style)
            && TryGetColor(*Style, Channel, Color))
        {
            return Color;
        }
    }

    return GetNativeColor(Node, Channel);
}

void FHueStyleResolver::SetColorOverride(
    UEdGraphNode* Node,
    EHueStyleScope Scope,
    EHueStyleChannel Channel,
    const FLinearColor& Color)
{
    if (!Node || !IsSupportedNode(Node))
    {
        return;
    }

    TUniquePtr<FScopedTransaction> Transaction;

    if (Scope != EHueStyleScope::GlobalFunction)
    {
        UBlueprint* Blueprint = GetScopeBlueprint(Node, Scope);
        if (!Blueprint)
        {
            return;
        }

        Transaction = MakeUnique<FScopedTransaction>(
            LOCTEXT("SetHueStyle", "Set Hue Style"));

        Blueprint->Modify();

        if (UHueBlueprintExtension* Extension = GetOrCreateBlueprintExtension(Blueprint))
        {
            Extension->Modify();
        }
    }

    if (FHueNodeStyleOverride* Style =
        HueStyleResolverPrivate::GetMutableScopeStyle(Node, Scope, true))
    {
        SetColor(*Style, Channel, Color);
        HueStyleResolverPrivate::SaveScope(Node, Scope);
        HueStyleResolverPrivate::InvalidateHueWidgets();
    }
}

void FHueStyleResolver::ClearColorOverride(
    UEdGraphNode* Node,
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    if (!Node || !IsSupportedNode(Node))
    {
        return;
    }

    TUniquePtr<FScopedTransaction> Transaction;

    if (Scope != EHueStyleScope::GlobalFunction)
    {
        UBlueprint* Blueprint = GetScopeBlueprint(Node, Scope);
        if (!Blueprint)
        {
            return;
        }

        Transaction = MakeUnique<FScopedTransaction>(
            LOCTEXT("ClearHueStyle", "Clear Hue Style"));

        Blueprint->Modify();

        if (UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint))
        {
            Extension->Modify();
        }
    }

    if (FHueNodeStyleOverride* Style =
        HueStyleResolverPrivate::GetMutableScopeStyle(Node, Scope, false))
    {
        ClearColor(*Style, Channel);
        HueStyleResolverPrivate::RemoveEmptyStyle(Node, Scope);
        HueStyleResolverPrivate::SaveScope(Node, Scope);
        HueStyleResolverPrivate::InvalidateHueWidgets();
    }
}

void FHueStyleResolver::ClearAllOverrides(
    UEdGraphNode* Node,
    EHueStyleScope Scope)
{
    if (!Node || !IsSupportedNode(Node))
    {
        return;
    }

    if (Scope == EHueStyleScope::GlobalFunction)
    {
        UHueSettings* Settings = GetMutableDefault<UHueSettings>();
        Settings->GlobalFunctionStyles.Remove(GetGlobalKey(Node));
        Settings->SaveConfig();
        HueStyleResolverPrivate::InvalidateHueWidgets();
        return;
    }

    UBlueprint* Blueprint = GetScopeBlueprint(Node, Scope);
    UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint);

    if (!Blueprint || !Extension)
    {
        return;
    }

    const FScopedTransaction Transaction(
        LOCTEXT("ClearHueStyleScope", "Clear Hue Style"));

    Blueprint->Modify();
    Extension->Modify();

    if (Scope == EHueStyleScope::Instance)
    {
        Extension->InstanceStyles.Remove(Node->NodeGuid);
    }
    else
    {
        FHueBlueprintCategoryInfo CategoryInfo;
        if (!GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return;
        }

        Extension->CategoryStyles.Remove(CategoryInfo.Category);
    }

    Blueprint->MarkPackageDirty();
    HueStyleResolverPrivate::InvalidateHueWidgets();
}

bool FHueStyleResolver::HasColorOverride(
    const UEdGraphNode* Node,
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    const FHueNodeStyleOverride* Style = nullptr;
    FLinearColor Ignored;

    return TryGetScopeStyle(Node, Scope, Style)
        && TryGetColor(*Style, Channel, Ignored);
}

bool FHueStyleResolver::HasAnyOverride(
    const UEdGraphNode* Node,
    EHueStyleScope Scope)
{
    const FHueNodeStyleOverride* Style = nullptr;
    return TryGetScopeStyle(Node, Scope, Style)
        && !Style->IsEmpty();
}

#undef LOCTEXT_NAMESPACE
