// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueStyleResolver.h"

#include "Blueprint/BlueprintExtension.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphSchema_K2.h"
#include "Engine/Blueprint.h"
#include "Framework/Application/SlateApplication.h"
#include "HueBlueprintExtension.h"
#include "HueNativeNodeDecorator.h"
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
#include "SGraphNode.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"

#define LOCTEXT_NAMESPACE "HueStyleResolver"

namespace HueStyleResolverPrivate
{
    static TSet<TWeakObjectPtr<UEdGraphNode>> VisuallySupportedNodes;
    static TMap<TWeakObjectPtr<UEdGraphNode>, TWeakPtr<SGraphNode>> RejectedNativeFallbackWidgets;

    static void RefreshCategoryTrackingAndMigrate(
        UBlueprint* Blueprint,
        UHueBlueprintExtension* Extension,
        const FString& CurrentCategory);

    static void PruneVisualSupportCaches()
    {
        for (auto It = VisuallySupportedNodes.CreateIterator(); It; ++It)
        {
            if (!It->IsValid())
            {
                It.RemoveCurrent();
            }
        }

        for (auto It = RejectedNativeFallbackWidgets.CreateIterator(); It; ++It)
        {
            if (!It.Key().IsValid() || !It.Value().IsValid())
            {
                It.RemoveCurrent();
            }
        }
    }

    static void ClearRejectedFallbacksForBlueprint(const UBlueprint* Blueprint)
    {
        if (!Blueprint)
        {
            return;
        }

        for (auto It = RejectedNativeFallbackWidgets.CreateIterator(); It; ++It)
        {
            UEdGraphNode* Node = It.Key().Get();
            if (!Node || FHueStyleResolver::GetOwningBlueprint(Node) == Blueprint)
            {
                It.RemoveCurrent();
            }
        }
    }

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

        RefreshCategoryTrackingAndMigrate(
            Blueprint,
            Extension,
            CategoryInfo.Category);

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

        RefreshCategoryTrackingAndMigrate(
            Blueprint,
            Extension,
            CategoryInfo.Category);

        if (FHueNodeStyleOverride* Style = Extension->CategoryStyles.Find(CategoryInfo.Category))
        {
            if (Style->IsEmpty())
            {
                Extension->CategoryStyles.Remove(CategoryInfo.Category);
            }
        }
    }


    static FString GetScopeTargetKey(
        const UEdGraphNode* Node,
        EHueStyleScope Scope)
    {
        if (!Node || !FHueStyleResolver::IsSupportedNode(Node))
        {
            return FString();
        }

        if (Scope == EHueStyleScope::Instance)
        {
            const UBlueprint* Blueprint = FHueStyleResolver::GetOwningBlueprint(Node);
            return Blueprint
                ? FString::Printf(
                    TEXT("Instance:%s:%s"),
                    *Blueprint->GetPathName(),
                    *Node->NodeGuid.ToString(EGuidFormats::DigitsWithHyphens))
                : FString();
        }

        if (Scope == EHueStyleScope::Category)
        {
            FHueBlueprintCategoryInfo CategoryInfo;
            if (!FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo)
                || !CategoryInfo.DefiningBlueprint)
            {
                return FString();
            }

            return FString::Printf(
                TEXT("Category:%s:%s"),
                *CategoryInfo.DefiningBlueprint->GetPathName(),
                *CategoryInfo.Category);
        }

        const FString GlobalKey = FHueStyleResolver::GetGlobalKey(Node);
        return GlobalKey.IsEmpty()
            ? FString()
            : FString(TEXT("Global:")) + GlobalKey;
    }

    static TArray<UEdGraphNode*> GetUniqueScopeRepresentatives(
        const TArray<UEdGraphNode*>& Nodes,
        EHueStyleScope Scope)
    {
        TArray<UEdGraphNode*> Result;
        TSet<FString> SeenTargets;

        for (UEdGraphNode* Node : Nodes)
        {
            const FString TargetKey = GetScopeTargetKey(Node, Scope);
            if (TargetKey.IsEmpty() || SeenTargets.Contains(TargetKey))
            {
                continue;
            }

            SeenTargets.Add(TargetKey);
            Result.Add(Node);
        }

        return Result;
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

    struct FCategoryTrackingState
    {
        TArray<FName> CategorySorting;
        TMap<FString, FString> MemberFingerprints;
    };

    static TMap<TWeakObjectPtr<UBlueprint>, FCategoryTrackingState> CategoryTrackingStates;

    static FString BuildCategoryMemberFingerprint(
        UBlueprint* Blueprint,
        const FString& Category)
    {
        if (!Blueprint || Category.IsEmpty())
        {
            return FString();
        }

        TArray<FString> MemberKeys;

        auto AddGraphIfCategoryMatches =
            [&MemberKeys, Blueprint, &Category](UEdGraph* Graph, const TCHAR* Prefix)
            {
                FHueBlueprintCategoryInfo Info;
                if (Graph
                    && ReadExplicitBlueprintCategory(Graph, Blueprint, Info)
                    && Info.Category == Category)
                {
                    MemberKeys.Add(
                        FString::Printf(
                            TEXT("%s%s"),
                            Prefix,
                            *Graph->GetName()));
                }
            };

        for (UEdGraph* Graph : Blueprint->FunctionGraphs)
        {
            AddGraphIfCategoryMatches(Graph, TEXT("Function:"));
        }

        for (UEdGraph* Graph : Blueprint->MacroGraphs)
        {
            AddGraphIfCategoryMatches(Graph, TEXT("Macro:"));
        }

        for (const FBPVariableDescription& Variable : Blueprint->NewVariables)
        {
            const FString VariableCategory =
                Variable.Category.ToString().TrimStartAndEnd();

            if (VariableCategory == Category)
            {
                MemberKeys.Add(
                    FString::Printf(
                        TEXT("Variable:%s"),
                        *Variable.VarGuid.ToString(EGuidFormats::DigitsWithHyphens)));
            }
        }

        TArray<UEdGraph*> AllGraphs;
        Blueprint->GetAllGraphs(AllGraphs);

        for (UEdGraph* Graph : AllGraphs)
        {
            if (!Graph)
            {
                continue;
            }

            for (UEdGraphNode* GraphNode : Graph->Nodes)
            {
                const UK2Node_Event* EventNode = Cast<UK2Node_Event>(GraphNode);
                if (!EventNode)
                {
                    continue;
                }

                FHueBlueprintCategoryInfo EventInfo;
                if (ReadExplicitBlueprintEventCategory(EventNode, EventInfo)
                    && EventInfo.DefiningBlueprint == Blueprint
                    && EventInfo.Category == Category)
                {
                    MemberKeys.Add(
                        FString::Printf(
                            TEXT("Event:%s"),
                            *EventNode->NodeGuid.ToString(EGuidFormats::DigitsWithHyphens)));
                }
            }
        }

        MemberKeys.Sort();
        return FString::Join(MemberKeys, TEXT("|"));
    }

    static int32 CountFingerprintMembers(const FString& Fingerprint)
    {
        if (Fingerprint.IsEmpty())
        {
            return 0;
        }

        int32 Count = 1;
        for (TCHAR Character : Fingerprint)
        {
            if (Character == TEXT('|'))
            {
                ++Count;
            }
        }
        return Count;
    }

    static void CollectBlueprintMemberCategories(
        UBlueprint* Blueprint,
        TSet<FString>& OutCategories)
    {
        OutCategories.Reset();

        if (!Blueprint)
        {
            return;
        }

        auto AddGraphCategory =
            [&OutCategories, Blueprint](UEdGraph* Graph)
            {
                FHueBlueprintCategoryInfo Info;
                if (Graph
                    && ReadExplicitBlueprintCategory(Graph, Blueprint, Info)
                    && !Info.Category.IsEmpty())
                {
                    OutCategories.Add(Info.Category);
                }
            };

        for (UEdGraph* Graph : Blueprint->FunctionGraphs)
        {
            AddGraphCategory(Graph);
        }

        for (UEdGraph* Graph : Blueprint->MacroGraphs)
        {
            AddGraphCategory(Graph);
        }

        for (const FBPVariableDescription& Variable : Blueprint->NewVariables)
        {
            const FString Category =
                Variable.Category.ToString().TrimStartAndEnd();
            if (!Category.IsEmpty()
                && !FText::FromString(Category).EqualTo(UEdGraphSchema_K2::VR_DefaultCategory))
            {
                OutCategories.Add(Category);
            }
        }

        TArray<UEdGraph*> AllGraphs;
        Blueprint->GetAllGraphs(AllGraphs);
        for (UEdGraph* Graph : AllGraphs)
        {
            if (!Graph)
            {
                continue;
            }

            for (UEdGraphNode* GraphNode : Graph->Nodes)
            {
                const UK2Node_Event* EventNode = Cast<UK2Node_Event>(GraphNode);
                if (!EventNode)
                {
                    continue;
                }

                FHueBlueprintCategoryInfo EventInfo;
                if (ReadExplicitBlueprintEventCategory(EventNode, EventInfo)
                    && EventInfo.DefiningBlueprint == Blueprint
                    && !EventInfo.Category.IsEmpty())
                {
                    OutCategories.Add(EventInfo.Category);
                }
            }
        }
    }

    static void MergeStyleWithoutOverwriting(
        FHueNodeStyleOverride& Destination,
        const FHueNodeStyleOverride& Source)
    {
        if (!Destination.bOverrideHeaderColor && Source.bOverrideHeaderColor)
        {
            Destination.bOverrideHeaderColor = true;
            Destination.HeaderColor = Source.HeaderColor;
        }

        if (!Destination.bOverrideHeaderTextColor && Source.bOverrideHeaderTextColor)
        {
            Destination.bOverrideHeaderTextColor = true;
            Destination.HeaderTextColor = Source.HeaderTextColor;
        }

        if (!Destination.bOverrideBodyColor && Source.bOverrideBodyColor)
        {
            Destination.bOverrideBodyColor = true;
            Destination.BodyColor = Source.BodyColor;
        }

        if (!Destination.bOverrideBodyTextColor && Source.bOverrideBodyTextColor)
        {
            Destination.bOverrideBodyTextColor = true;
            Destination.BodyTextColor = Source.BodyTextColor;
        }
    }

    static bool MigrateCategoryStyle(
        UBlueprint* Blueprint,
        UHueBlueprintExtension* Extension,
        const FString& OldCategory,
        const FString& NewCategory)
    {
        if (!Blueprint
            || !Extension
            || OldCategory.IsEmpty()
            || NewCategory.IsEmpty()
            || OldCategory == NewCategory)
        {
            return false;
        }

        const FHueNodeStyleOverride* OldStyle =
            Extension->CategoryStyles.Find(OldCategory);
        if (!OldStyle)
        {
            return false;
        }

        const FHueNodeStyleOverride StyleCopy = *OldStyle;

        Blueprint->Modify();
        Extension->Modify();

        if (FHueNodeStyleOverride* ExistingTarget =
            Extension->CategoryStyles.Find(NewCategory))
        {
            // A rename can merge into an existing category. Preserve the
            // destination rule on conflicts and fill only channels it did not
            // already override.
            MergeStyleWithoutOverwriting(*ExistingTarget, StyleCopy);
        }
        else
        {
            Extension->CategoryStyles.Add(NewCategory, StyleCopy);
        }

        Extension->CategoryStyles.Remove(OldCategory);
        Blueprint->MarkPackageDirty();
        return true;
    }

    struct FCategoryRenamePair
    {
        FString OldCategory;
        FString NewCategory;

        bool operator==(const FCategoryRenamePair& Other) const
        {
            return OldCategory == Other.OldCategory
                && NewCategory == Other.NewCategory;
        }
    };

    static TArray<FCategoryRenamePair> DetectCategorySortRenames(
        const TArray<FName>& PreviousSorting,
        const TArray<FName>& CurrentSorting)
    {
        TSet<FName> PreviousSet;
        TSet<FName> CurrentSet;
        for (const FName& Name : PreviousSorting)
        {
            PreviousSet.Add(Name);
        }
        for (const FName& Name : CurrentSorting)
        {
            CurrentSet.Add(Name);
        }

        TArray<FName> Removed;
        TArray<FName> Added;
        for (const FName& Name : PreviousSorting)
        {
            if (!CurrentSet.Contains(Name))
            {
                Removed.Add(Name);
            }
        }
        for (const FName& Name : CurrentSorting)
        {
            if (!PreviousSet.Contains(Name))
            {
                Added.Add(Name);
            }
        }

        TArray<FCategoryRenamePair> Result;

        if (Removed.Num() == 1 && Added.Num() == 1)
        {
            FCategoryRenamePair Pair;
            Pair.OldCategory = Removed[0].ToString();
            Pair.NewCategory = Added[0].ToString();
            Result.Add(Pair);
            return Result;
        }

        // A category rename normally preserves its sort slot. This also lets
        // nested-category renames produce several reliable old/new pairs while
        // a pure reorder (same name set) produces none.
        const int32 CommonCount = FMath::Min(
            PreviousSorting.Num(),
            CurrentSorting.Num());

        for (int32 Index = 0; Index < CommonCount; ++Index)
        {
            const FName OldName = PreviousSorting[Index];
            const FName NewName = CurrentSorting[Index];

            if (OldName != NewName
                && !CurrentSet.Contains(OldName)
                && !PreviousSet.Contains(NewName))
            {
                FCategoryRenamePair Pair;
                Pair.OldCategory = OldName.ToString();
                Pair.NewCategory = NewName.ToString();
                Result.AddUnique(Pair);
            }
        }

        return Result;
    }

    static bool TryApplyCategoryRenamePairs(
        UBlueprint* Blueprint,
        UHueBlueprintExtension* Extension,
        const TArray<FCategoryRenamePair>& RenamePairs)
    {
        if (!Blueprint || !Extension || RenamePairs.IsEmpty())
        {
            return false;
        }

        TArray<FString> ExistingStyleKeys;
        Extension->CategoryStyles.GetKeys(ExistingStyleKeys);

        bool bMigratedAny = false;

        for (const FString& ExistingKey : ExistingStyleKeys)
        {
            for (const FCategoryRenamePair& Pair : RenamePairs)
            {
                FString TargetKey;

                if (ExistingKey == Pair.OldCategory)
                {
                    TargetKey = Pair.NewCategory;
                }
                else
                {
                    const FString OldPrefix = Pair.OldCategory + TEXT("|");
                    if (ExistingKey.StartsWith(OldPrefix))
                    {
                        TargetKey = Pair.NewCategory + ExistingKey.Mid(Pair.OldCategory.Len());
                    }
                }

                if (!TargetKey.IsEmpty())
                {
                    bMigratedAny |= MigrateCategoryStyle(
                        Blueprint,
                        Extension,
                        ExistingKey,
                        TargetKey);
                    break;
                }
            }
        }

        return bMigratedAny;
    }

    static void CaptureCategoryTrackingState(
        UBlueprint* Blueprint,
        UHueBlueprintExtension* Extension,
        FCategoryTrackingState& State)
    {
        if (!Blueprint || !Extension)
        {
            return;
        }

        State.CategorySorting = Blueprint->CategorySorting;
        State.MemberFingerprints.Reset();

        for (const TPair<FString, FHueNodeStyleOverride>& Pair :
            Extension->CategoryStyles)
        {
            State.MemberFingerprints.Add(
                Pair.Key,
                BuildCategoryMemberFingerprint(Blueprint, Pair.Key));
        }
    }

    static void RefreshCategoryTrackingAndMigrate(
        UBlueprint* Blueprint,
        UHueBlueprintExtension* Extension,
        const FString& CurrentCategory)
    {
        if (!Blueprint || !Extension)
        {
            return;
        }

        // Opportunistically prune unloaded Blueprint keys.
        for (auto It = CategoryTrackingStates.CreateIterator(); It; ++It)
        {
            if (!It.Key().IsValid())
            {
                It.RemoveCurrent();
            }
        }

        const TWeakObjectPtr<UBlueprint> BlueprintKey(Blueprint);
        FCategoryTrackingState* ExistingState =
            CategoryTrackingStates.Find(BlueprintKey);

        if (!ExistingState)
        {
            FCategoryTrackingState NewState;
            CaptureCategoryTrackingState(Blueprint, Extension, NewState);
            CategoryTrackingStates.Add(BlueprintKey, MoveTemp(NewState));
            return;
        }

        const TArray<FCategoryRenamePair> RenamePairs =
            DetectCategorySortRenames(
                ExistingState->CategorySorting,
                Blueprint->CategorySorting);

        bool bMigrated = TryApplyCategoryRenamePairs(
            Blueprint,
            Extension,
            RenamePairs);

        // Some Blueprints do not have useful entries in CategorySorting. As a
        // fallback, compare the durable members that made up a styled category.
        // Requiring at least two members avoids treating a one-member drag to a
        // different category as a category rename. Single-member renames still
        // migrate when Unreal records the rename in CategorySorting.
        if (!CurrentCategory.IsEmpty()
            && !Extension->CategoryStyles.Contains(CurrentCategory))
        {
            const FString CurrentFingerprint =
                BuildCategoryMemberFingerprint(Blueprint, CurrentCategory);

            if (!CurrentFingerprint.IsEmpty())
            {
                TArray<FString> ExistingStyleKeys;
                Extension->CategoryStyles.GetKeys(ExistingStyleKeys);

                for (const FString& OldCategory : ExistingStyleKeys)
                {
                    if (OldCategory == CurrentCategory)
                    {
                        continue;
                    }

                    const FString* PreviousFingerprint =
                        ExistingState->MemberFingerprints.Find(OldCategory);

                    if (!PreviousFingerprint
                        || *PreviousFingerprint != CurrentFingerprint
                        || CountFingerprintMembers(*PreviousFingerprint) < 2)
                    {
                        continue;
                    }

                    const FString OldCurrentFingerprint =
                        BuildCategoryMemberFingerprint(Blueprint, OldCategory);

                    if (!OldCurrentFingerprint.IsEmpty())
                    {
                        continue;
                    }

                    bMigrated |= MigrateCategoryStyle(
                        Blueprint,
                        Extension,
                        OldCategory,
                        CurrentCategory);
                    break;
                }
            }
        }
        else if (CurrentCategory.IsEmpty() && !bMigrated)
        {
            // Event-driven refreshes do not have one current node/category to
            // nominate. Preserve the old fingerprint fallback by comparing the
            // previously styled category membership against every category that
            // exists after the Blueprint change. Requiring at least two members
            // keeps a one-member category move from being guessed as a rename.
            TSet<FString> CurrentCategories;
            CollectBlueprintMemberCategories(Blueprint, CurrentCategories);

            TArray<FString> ExistingStyleKeys;
            Extension->CategoryStyles.GetKeys(ExistingStyleKeys);

            for (const FString& OldCategory : ExistingStyleKeys)
            {
                const FString* PreviousFingerprint =
                    ExistingState->MemberFingerprints.Find(OldCategory);
                if (!PreviousFingerprint
                    || PreviousFingerprint->IsEmpty()
                    || CountFingerprintMembers(*PreviousFingerprint) < 2)
                {
                    continue;
                }

                if (!BuildCategoryMemberFingerprint(Blueprint, OldCategory).IsEmpty())
                {
                    continue;
                }

                FString MatchingCategory;
                int32 MatchingCount = 0;
                for (const FString& Candidate : CurrentCategories)
                {
                    if (Candidate == OldCategory
                        || Extension->CategoryStyles.Contains(Candidate))
                    {
                        continue;
                    }

                    if (BuildCategoryMemberFingerprint(Blueprint, Candidate)
                        == *PreviousFingerprint)
                    {
                        MatchingCategory = Candidate;
                        ++MatchingCount;
                        if (MatchingCount > 1)
                        {
                            break;
                        }
                    }
                }

                if (MatchingCount == 1)
                {
                    bMigrated |= MigrateCategoryStyle(
                        Blueprint,
                        Extension,
                        OldCategory,
                        MatchingCategory);
                }
            }
        }

        CaptureCategoryTrackingState(Blueprint, Extension, *ExistingState);

        if (bMigrated)
        {
            InvalidateHueWidgets();
        }
    }
}

bool FHueStyleResolver::UsesSelfVisualBridge(const UEdGraphNode* Node)
{
    const UK2Node* K2Node = Cast<UK2Node>(Node);
    if (!K2Node)
    {
        return false;
    }

    // Create Widget owns its visual widget and therefore bypasses registered
    // FGraphPanelNodeFactory implementations. Keep this name-based on purpose:
    // UK2Node_CreateWidget lives in UMGEditor, and Hue should not need a hard
    // UMGEditor dependency just to decorate the finished native Slate widget.
    static const FName CreateWidgetClassName(TEXT("K2Node_CreateWidget"));
    return K2Node->GetClass()->GetFName() == CreateWidgetClassName;
}

void FHueStyleResolver::MarkVisualSupport(UEdGraphNode* Node)
{
    if (!Node)
    {
        return;
    }

    const TWeakObjectPtr<UEdGraphNode> WeakNode(Node);
    HueStyleResolverPrivate::VisuallySupportedNodes.Add(WeakNode);
    HueStyleResolverPrivate::RejectedNativeFallbackWidgets.Remove(WeakNode);
    HueStyleResolverPrivate::PruneVisualSupportCaches();
}

void FHueStyleResolver::UnmarkVisualSupport(UEdGraphNode* Node)
{
    if (!Node)
    {
        return;
    }

    const TWeakObjectPtr<UEdGraphNode> WeakNode(Node);
    HueStyleResolverPrivate::VisuallySupportedNodes.Remove(WeakNode);

    // If Unreal already produced a displayed widget and Hue could not decorate
    // it, remember that negative result. Otherwise IsSupportedNode() would walk
    // the same Slate subtree again every panel tick and context-menu query.
    if (TSharedPtr<SGraphNode> DisplayedWidget = Node->DEPRECATED_NodeWidget.Pin())
    {
        HueStyleResolverPrivate::RejectedNativeFallbackWidgets.Add(
            WeakNode,
            DisplayedWidget);
    }
}

bool FHueStyleResolver::IsSupportedNode(const UEdGraphNode* Node)
{
    if (!CanStyleNode(Node))
    {
        return false;
    }

    const TWeakObjectPtr<UEdGraphNode> WeakNode(
        const_cast<UEdGraphNode*>(Node));

    if (HueStyleResolverPrivate::VisuallySupportedNodes.Contains(WeakNode))
    {
        return true;
    }

    if (TWeakPtr<SGraphNode>* RejectedWidget =
        HueStyleResolverPrivate::RejectedNativeFallbackWidgets.Find(WeakNode))
    {
        const TSharedPtr<SGraphNode> CurrentWidget =
            const_cast<UEdGraphNode*>(Node)->DEPRECATED_NodeWidget.Pin();
        const TSharedPtr<SGraphNode> PreviousRejectedWidget = RejectedWidget->Pin();

        // Keep the negative result only for the exact Slate presentation that
        // failed. If another graph panel or reconstruction gives the same node
        // a different widget, allow that presentation to prove compatibility.
        if (CurrentWidget.IsValid()
            && PreviousRejectedWidget.IsValid()
            && CurrentWidget == PreviousRejectedWidget)
        {
            return false;
        }

        HueStyleResolverPrivate::RejectedNativeFallbackWidgets.Remove(WeakNode);
    }

    // A K2 node can bypass every registered graph-node factory by returning
    // its own widget from UEdGraphNode::CreateVisualWidget(). When the native
    // widget is already on screen, opportunistically test whether it exposes
    // the standard GraphEditor header/body layers Hue knows how to decorate.
    // The decorator only reports success after both surfaces are found, so an
    // arbitrary custom widget cannot accidentally receive Hue controls that do
    // nothing. This also gives future engine/third-party node-owned widgets a
    // compatibility path without hardcoding each class name.
    UK2Node* MutableK2Node = Cast<UK2Node>(
        const_cast<UEdGraphNode*>(Node));
    if (MutableK2Node)
    {
        const bool bHasDisplayedWidget =
            MutableK2Node->DEPRECATED_NodeWidget.IsValid();

        if (FHueNativeNodeDecorator::ApplyToDisplayedNode(MutableK2Node))
        {
            MarkVisualSupport(MutableK2Node);
            return true;
        }

        if (bHasDisplayedWidget)
        {
            if (TSharedPtr<SGraphNode> DisplayedWidget =
                MutableK2Node->DEPRECATED_NodeWidget.Pin())
            {
                HueStyleResolverPrivate::RejectedNativeFallbackWidgets.Add(
                    WeakNode,
                    DisplayedWidget);
            }
        }
    }

    return false;
}

bool FHueStyleResolver::CanStyleNode(const UEdGraphNode* Node)
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

void FHueStyleResolver::PrimeCategoryTracking(UBlueprint* Blueprint)
{
    if (!Blueprint)
    {
        return;
    }

    if (UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint))
    {
        HueStyleResolverPrivate::RefreshCategoryTrackingAndMigrate(
            Blueprint,
            Extension,
            FString());
    }
}

void FHueStyleResolver::NotifyBlueprintChanged(
    UBlueprint* Blueprint,
    bool bPresentationMayHaveRebuilt)
{
    if (!Blueprint)
    {
        return;
    }

    // A compile/reconstruction can replace a custom node's displayed Slate
    // widget while retaining the UObject node. Only clear negative presentation
    // probes for those rebuild boundaries. Ordinary Blueprint changes should
    // keep the cache hot.
    if (bPresentationMayHaveRebuilt)
    {
        HueStyleResolverPrivate::ClearRejectedFallbacksForBlueprint(Blueprint);
    }
    HueStyleResolverPrivate::PruneVisualSupportCaches();

    if (UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint))
    {
        HueStyleResolverPrivate::RefreshCategoryTrackingAndMigrate(
            Blueprint,
            Extension,
            FString());
    }
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

    // Category rename migration is driven by UBlueprint change notifications.
    // Keep normal style resolution lookup-only so Slate paint/layout requests
    // never scan the Blueprint's category/member structure.
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

    if (GetEffectiveCategoryColor(Node, Channel, OutColor))
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
        if (GetEffectiveCategoryColor(Node, Channel, Color))
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
        // The exact Category was checked above. If this channel is not
        // explicitly overridden there, inherit from the nearest parent before
        // falling through to the Global rule.
        if (GetEffectiveCategoryColor(Node, Channel, Color, nullptr, false))
        {
            return Color;
        }

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
    TArray<UEdGraphNode*> Nodes;
    if (Node)
    {
        Nodes.Add(Node);
    }
    SetColorOverrides(Nodes, Scope, Channel, Color);
}

void FHueStyleResolver::ClearColorOverride(
    UEdGraphNode* Node,
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    TArray<UEdGraphNode*> Nodes;
    if (Node)
    {
        Nodes.Add(Node);
    }
    ClearColorOverrides(Nodes, Scope, Channel);
}

void FHueStyleResolver::ClearAllOverrides(
    UEdGraphNode* Node,
    EHueStyleScope Scope)
{
    TArray<UEdGraphNode*> Nodes;
    if (Node)
    {
        Nodes.Add(Node);
    }
    ClearAllOverrides(Nodes, Scope);
}

void FHueStyleResolver::SetColorOverrides(
    const TArray<UEdGraphNode*>& Nodes,
    EHueStyleScope Scope,
    EHueStyleChannel Channel,
    const FLinearColor& Color)
{
    const TArray<UEdGraphNode*> Targets =
        HueStyleResolverPrivate::GetUniqueScopeRepresentatives(Nodes, Scope);

    if (Targets.IsEmpty())
    {
        return;
    }

    const FScopedTransaction Transaction(
        Targets.Num() > 1
            ? LOCTEXT("SetHueStylesBatch", "Set Hue Styles")
            : LOCTEXT("SetHueStyle", "Set Hue Style"));

    TSet<UBlueprint*> ModifiedBlueprints;
    UHueSettings* Settings = nullptr;

    if (Scope == EHueStyleScope::GlobalFunction)
    {
        Settings = GetMutableDefault<UHueSettings>();
        Settings->SetFlags(RF_Transactional);
        Settings->Modify();
    }

    for (UEdGraphNode* Node : Targets)
    {
        if (Scope != EHueStyleScope::GlobalFunction)
        {
            UBlueprint* Blueprint = GetScopeBlueprint(Node, Scope);
            if (!Blueprint)
            {
                continue;
            }

            if (!ModifiedBlueprints.Contains(Blueprint))
            {
                Blueprint->Modify();
                ModifiedBlueprints.Add(Blueprint);
            }

            if (UHueBlueprintExtension* Extension =
                GetOrCreateBlueprintExtension(Blueprint))
            {
                Extension->Modify();
            }
        }

        if (FHueNodeStyleOverride* Style =
            HueStyleResolverPrivate::GetMutableScopeStyle(Node, Scope, true))
        {
            SetColor(*Style, Channel, Color);
        }
    }

    if (Settings)
    {
        Settings->SaveConfig();
    }

    for (UBlueprint* Blueprint : ModifiedBlueprints)
    {
        if (Scope == EHueStyleScope::Category)
        {
            if (UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint))
            {
                HueStyleResolverPrivate::RefreshCategoryTrackingAndMigrate(
                    Blueprint,
                    Extension,
                    FString());
            }
        }

        Blueprint->MarkPackageDirty();
    }

    HueStyleResolverPrivate::InvalidateHueWidgets();
}

void FHueStyleResolver::ClearColorOverrides(
    const TArray<UEdGraphNode*>& Nodes,
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    const TArray<UEdGraphNode*> Targets =
        HueStyleResolverPrivate::GetUniqueScopeRepresentatives(Nodes, Scope);

    if (Targets.IsEmpty())
    {
        return;
    }

    const FScopedTransaction Transaction(
        Targets.Num() > 1
            ? LOCTEXT("ClearHueStylesBatch", "Clear Hue Styles")
            : LOCTEXT("ClearHueStyle", "Clear Hue Style"));

    TSet<UBlueprint*> ModifiedBlueprints;
    UHueSettings* Settings = nullptr;

    if (Scope == EHueStyleScope::GlobalFunction)
    {
        Settings = GetMutableDefault<UHueSettings>();
        Settings->SetFlags(RF_Transactional);
        Settings->Modify();
    }

    for (UEdGraphNode* Node : Targets)
    {
        if (Scope != EHueStyleScope::GlobalFunction)
        {
            UBlueprint* Blueprint = GetScopeBlueprint(Node, Scope);
            if (!Blueprint)
            {
                continue;
            }

            if (!ModifiedBlueprints.Contains(Blueprint))
            {
                Blueprint->Modify();
                ModifiedBlueprints.Add(Blueprint);
            }

            if (UHueBlueprintExtension* Extension =
                FindBlueprintExtension(Blueprint))
            {
                Extension->Modify();
            }
        }

        if (FHueNodeStyleOverride* Style =
            HueStyleResolverPrivate::GetMutableScopeStyle(Node, Scope, false))
        {
            ClearColor(*Style, Channel);
            HueStyleResolverPrivate::RemoveEmptyStyle(Node, Scope);
        }
    }

    if (Settings)
    {
        Settings->SaveConfig();
    }

    for (UBlueprint* Blueprint : ModifiedBlueprints)
    {
        if (Scope == EHueStyleScope::Category)
        {
            if (UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint))
            {
                HueStyleResolverPrivate::RefreshCategoryTrackingAndMigrate(
                    Blueprint,
                    Extension,
                    FString());
            }
        }

        Blueprint->MarkPackageDirty();
    }

    HueStyleResolverPrivate::InvalidateHueWidgets();
}

void FHueStyleResolver::ClearAllOverrides(
    const TArray<UEdGraphNode*>& Nodes,
    EHueStyleScope Scope)
{
    const TArray<UEdGraphNode*> Targets =
        HueStyleResolverPrivate::GetUniqueScopeRepresentatives(Nodes, Scope);

    if (Targets.IsEmpty())
    {
        return;
    }

    const FScopedTransaction Transaction(
        Targets.Num() > 1
            ? LOCTEXT("ClearHueScopesBatch", "Clear Hue Overrides")
            : LOCTEXT("ClearHueStyleScope", "Clear Hue Style"));

    TSet<UBlueprint*> ModifiedBlueprints;
    UHueSettings* Settings = nullptr;

    if (Scope == EHueStyleScope::GlobalFunction)
    {
        Settings = GetMutableDefault<UHueSettings>();
        Settings->SetFlags(RF_Transactional);
        Settings->Modify();
    }

    for (UEdGraphNode* Node : Targets)
    {
        if (Scope == EHueStyleScope::GlobalFunction)
        {
            Settings->GlobalFunctionStyles.Remove(GetGlobalKey(Node));
            continue;
        }

        UBlueprint* Blueprint = GetScopeBlueprint(Node, Scope);
        UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint);

        if (!Blueprint || !Extension)
        {
            continue;
        }

        if (!ModifiedBlueprints.Contains(Blueprint))
        {
            Blueprint->Modify();
            ModifiedBlueprints.Add(Blueprint);
        }

        Extension->Modify();

        if (Scope == EHueStyleScope::Instance)
        {
            Extension->InstanceStyles.Remove(Node->NodeGuid);
        }
        else
        {
            FHueBlueprintCategoryInfo CategoryInfo;
            if (GetBlueprintCategoryInfo(Node, CategoryInfo))
            {
                HueStyleResolverPrivate::RefreshCategoryTrackingAndMigrate(
                    Blueprint,
                    Extension,
                    CategoryInfo.Category);
                Extension->CategoryStyles.Remove(CategoryInfo.Category);
            }
        }
    }

    if (Settings)
    {
        Settings->SaveConfig();
    }

    for (UBlueprint* Blueprint : ModifiedBlueprints)
    {
        if (Scope == EHueStyleScope::Category)
        {
            if (UHueBlueprintExtension* Extension = FindBlueprintExtension(Blueprint))
            {
                HueStyleResolverPrivate::RefreshCategoryTrackingAndMigrate(
                    Blueprint,
                    Extension,
                    FString());
            }
        }

        Blueprint->MarkPackageDirty();
    }

    HueStyleResolverPrivate::InvalidateHueWidgets();
}

bool FHueStyleResolver::GetEffectiveCategoryColor(
    const UEdGraphNode* Node,
    EHueStyleChannel Channel,
    FLinearColor& OutColor,
    FString* OutSourceCategory,
    bool bIncludeExact)
{
    if (OutSourceCategory)
    {
        OutSourceCategory->Reset();
    }

    FHueBlueprintCategoryInfo CategoryInfo;
    if (!GetBlueprintCategoryInfo(Node, CategoryInfo)
        || !CategoryInfo.DefiningBlueprint)
    {
        return false;
    }

    UHueBlueprintExtension* Extension =
        FindBlueprintExtension(CategoryInfo.DefiningBlueprint);
    if (!Extension)
    {
        return false;
    }

    // This is called by live Slate color attributes. Do not run category
    // migration here. Open Blueprint editors notify Hue when Blueprint data
    // changes, so this path stays a cheap hierarchy lookup.
    FString Candidate = CategoryInfo.Category.TrimStartAndEnd();

    if (!bIncludeExact)
    {
        int32 SeparatorIndex = INDEX_NONE;
        if (!Candidate.FindLastChar(TEXT('|'), SeparatorIndex))
        {
            return false;
        }
        Candidate = Candidate.Left(SeparatorIndex).TrimStartAndEnd();
    }

    while (!Candidate.IsEmpty())
    {
        if (const FHueNodeStyleOverride* CategoryStyle =
            Extension->CategoryStyles.Find(Candidate))
        {
            if (TryGetColor(*CategoryStyle, Channel, OutColor))
            {
                if (OutSourceCategory)
                {
                    *OutSourceCategory = Candidate;
                }
                return true;
            }
        }

        int32 SeparatorIndex = INDEX_NONE;
        if (!Candidate.FindLastChar(TEXT('|'), SeparatorIndex))
        {
            break;
        }

        Candidate = Candidate.Left(SeparatorIndex).TrimStartAndEnd();
    }

    return false;
}

bool FHueStyleResolver::GetColorOverride(
    const UEdGraphNode* Node,
    EHueStyleScope Scope,
    EHueStyleChannel Channel,
    FLinearColor& OutColor)
{
    const FHueNodeStyleOverride* Style = nullptr;
    return TryGetScopeStyle(Node, Scope, Style)
        && TryGetColor(*Style, Channel, OutColor);
}

int32 FHueStyleResolver::GetUniqueScopeTargetCount(
    const TArray<UEdGraphNode*>& Nodes,
    EHueStyleScope Scope)
{
    return HueStyleResolverPrivate::GetUniqueScopeRepresentatives(Nodes, Scope).Num();
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
