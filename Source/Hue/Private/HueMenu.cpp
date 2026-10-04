// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueMenu.h"

#include "BlueprintEditor.h"
#include "EdGraph/EdGraphNode.h"
#include "Editor.h"
#include "Engine/Blueprint.h"
#include "HueStyleResolver.h"
#include "Styling/AppStyle.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "ToolMenu.h"
#include "ToolMenuEntry.h"
#include "ToolMenuSection.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HueMenu"

namespace HueMenuPrivate
{
    static FName MakeEntryName(FName Prefix, const TCHAR* Suffix)
    {
        return FName(*(Prefix.ToString() + Suffix));
    }

    static TArray<UEdGraphNode*> GetLiveNodes(
        const TArray<TWeakObjectPtr<UEdGraphNode>>& WeakNodes)
    {
        TArray<UEdGraphNode*> Result;
        Result.Reserve(WeakNodes.Num());

        for (const TWeakObjectPtr<UEdGraphNode>& WeakNode : WeakNodes)
        {
            if (UEdGraphNode* Node = WeakNode.Get())
            {
                if (FHueStyleResolver::IsSupportedNode(Node))
                {
                    Result.Add(Node);
                }
            }
        }

        return Result;
    }

    static bool IsScopeApplicable(
        const UEdGraphNode* Node,
        EHueStyleScope Scope)
    {
        if (!Node || !FHueStyleResolver::IsSupportedNode(Node))
        {
            return false;
        }

        if (Scope == EHueStyleScope::Category)
        {
            FHueBlueprintCategoryInfo CategoryInfo;
            return FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo);
        }

        if (Scope == EHueStyleScope::GlobalFunction)
        {
            return !FHueStyleResolver::GetGlobalKey(Node).IsEmpty();
        }

        return true;
    }

    static bool HasAnyOverride(
        const TArray<UEdGraphNode*>& Nodes,
        EHueStyleScope Scope)
    {
        for (UEdGraphNode* Node : Nodes)
        {
            if (IsScopeApplicable(Node, Scope)
                && FHueStyleResolver::HasAnyOverride(Node, Scope))
            {
                return true;
            }
        }

        return false;
    }

    static bool HasColorOverride(
        const TArray<UEdGraphNode*>& Nodes,
        EHueStyleScope Scope,
        EHueStyleChannel Channel)
    {
        for (UEdGraphNode* Node : Nodes)
        {
            if (IsScopeApplicable(Node, Scope)
                && FHueStyleResolver::HasColorOverride(Node, Scope, Channel))
            {
                return true;
            }
        }

        return false;
    }

    static FText ScopeLabel(
        const TArray<UEdGraphNode*>& Nodes,
        EHueStyleScope Scope)
    {
        if (Nodes.Num() == 1)
        {
            switch (Scope)
            {
            case EHueStyleScope::Instance:
                return LOCTEXT("InstanceScope", "Instance");
            case EHueStyleScope::Category:
                return LOCTEXT("CategoryScope", "Category");
            case EHueStyleScope::GlobalFunction:
                return FHueStyleResolver::GetGlobalScopeLabel(Nodes[0]);
            }
        }

        const int32 Count =
            FHueStyleResolver::GetUniqueScopeTargetCount(Nodes, Scope);

        switch (Scope)
        {
        case EHueStyleScope::Instance:
            return FText::Format(
                LOCTEXT("InstanceBatchScope", "Instance ({0} Nodes)"),
                FText::AsNumber(Count));
        case EHueStyleScope::Category:
            return FText::Format(
                LOCTEXT("CategoryBatchScope", "Category ({0} Categories)"),
                FText::AsNumber(Count));
        case EHueStyleScope::GlobalFunction:
            return FText::Format(
                LOCTEXT("GlobalBatchScope", "Global ({0} Targets)"),
                FText::AsNumber(Count));
        }

        return FText::GetEmpty();
    }

    static TSharedRef<SWidget> MakeColorEntryWidget(
        const FText& Label,
        const FText& Tooltip,
        const FLinearColor& Color)
    {
        return SNew(SBox)
            .ToolTipText(Tooltip)
            .Padding(FMargin(2.0f, 1.0f))
            [
                SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(0.0f, 0.0f, 7.0f, 0.0f)
                [
                    SNew(SColorBlock)
                    .Color(Color)
                    .Size(FVector2D(14.0f, 14.0f))
                    .ShowBackgroundForAlpha(false)
                ]
                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                    .Text(Label)
                    .TextStyle(FAppStyle::Get(), "Menu.Label")
                ]
            ];
    }
}

TArray<TWeakObjectPtr<UEdGraphNode>> FHueMenu::ResolveContextNodes(
    UEdGraphNode* ContextNode)
{
    TArray<TWeakObjectPtr<UEdGraphNode>> Result;

    if (!ContextNode || !FHueStyleResolver::IsSupportedNode(ContextNode))
    {
        return Result;
    }

    // Default to the node that opened the menu. If that node is part of the
    // current Blueprint graph selection, expand the action to the full
    // Hue-compatible selection.
    Result.Add(ContextNode);

    UBlueprint* Blueprint =
        FHueStyleResolver::GetOwningBlueprint(ContextNode);

    if (!GEditor || !Blueprint)
    {
        return Result;
    }

    UAssetEditorSubsystem* AssetEditorSubsystem =
        GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();

    if (!AssetEditorSubsystem)
    {
        return Result;
    }

    IAssetEditorInstance* EditorInstance =
        AssetEditorSubsystem->FindEditorForAsset(Blueprint, false);

    if (!EditorInstance)
    {
        return Result;
    }

    // Blueprint assets are hosted by FBlueprintEditor or one of its derived
    // editor types. This is the same public editor interface Hue already uses
    // for its dockable panel.
    FBlueprintEditor* BlueprintEditor =
        static_cast<FBlueprintEditor*>(EditorInstance);

    if (!BlueprintEditor)
    {
        return Result;
    }

    const FGraphPanelSelectionSet Selection =
        BlueprintEditor->GetSelectedNodes();

    bool bContextNodeSelected = false;
    TArray<TWeakObjectPtr<UEdGraphNode>> SupportedSelection;

    for (UObject* SelectedObject : Selection)
    {
        UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(SelectedObject);
        if (!SelectedNode)
        {
            continue;
        }

        if (SelectedNode == ContextNode)
        {
            bContextNodeSelected = true;
        }

        if (FHueStyleResolver::IsSupportedNode(SelectedNode))
        {
            SupportedSelection.Add(SelectedNode);
        }
    }

    if (bContextNodeSelected && SupportedSelection.Num() > 0)
    {
        Result = MoveTemp(SupportedSelection);
    }

    return Result;
}

void FHueMenu::BuildNodeContextEntry(FToolMenuSection& Section)
{
    UGraphNodeContextMenuContext* Context =
        Section.FindContext<UGraphNodeContextMenuContext>();

    if (!Context || Context->bIsDebugging)
    {
        return;
    }

    UEdGraphNode* ContextNode =
        const_cast<UEdGraphNode*>(Context->Node.Get());

    TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes =
        ResolveContextNodes(ContextNode);

    if (WeakNodes.IsEmpty())
    {
        return;
    }

    const int32 NodeCount = WeakNodes.Num();

    Section.AddSubMenu(
        TEXT("Hue"),
        LOCTEXT("HueSubMenu", "Hue"),
        NodeCount > 1
            ? FText::Format(
                LOCTEXT(
                    "HueBatchSubMenuTooltip",
                    "Edit Hue properties for {0} selected compatible nodes as a batch."),
                FText::AsNumber(NodeCount))
            : LOCTEXT(
                "HueSubMenuTooltip",
                "Set visual style overrides for this Blueprint node."),
        FNewToolMenuChoice(
            FNewToolMenuDelegate::CreateLambda(
                [WeakNodes](UToolMenu* Menu)
                {
                    BuildHueMenu(Menu, WeakNodes);
                })),
        false,
        FSlateIcon(),
        true,
        NAME_None);
}

void FHueMenu::BuildHueMenu(
    UToolMenu* Menu,
    TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes)
{
    const TArray<UEdGraphNode*> Nodes =
        HueMenuPrivate::GetLiveNodes(WeakNodes);

    if (!Menu || Nodes.IsEmpty())
    {
        return;
    }

    FToolMenuSection& Section =
        Menu->FindOrAddSection(TEXT("HueScopes"));

    auto AddScope = [&Section, WeakNodes](
        EHueStyleScope Scope,
        FName Name,
        const FText& Label,
        const FText& Tooltip)
    {
        Section.AddSubMenu(
            Name,
            Label,
            Tooltip,
            FNewToolMenuChoice(
                FNewToolMenuDelegate::CreateLambda(
                    [WeakNodes, Scope](UToolMenu* ScopeMenu)
                    {
                        BuildScopeMenu(ScopeMenu, WeakNodes, Scope);
                    })),
            false,
            FSlateIcon(),
            true,
            NAME_None);
    };

    AddScope(
        EHueStyleScope::Instance,
        TEXT("HueInstance"),
        HueMenuPrivate::ScopeLabel(Nodes, EHueStyleScope::Instance),
        Nodes.Num() > 1
            ? LOCTEXT(
                "InstanceBatchTip",
                "Style every selected Hue-compatible node instance.")
            : LOCTEXT(
                "InstanceTip",
                "Style only this node instance."));

    const int32 CategoryCount =
        FHueStyleResolver::GetUniqueScopeTargetCount(
            Nodes,
            EHueStyleScope::Category);

    if (CategoryCount > 0)
    {
        FText CategoryLabel;

        if (Nodes.Num() == 1)
        {
            FHueBlueprintCategoryInfo CategoryInfo;
            FHueStyleResolver::GetBlueprintCategoryInfo(
                Nodes[0],
                CategoryInfo);

            CategoryLabel = FText::Format(
                LOCTEXT("CategoryFmt", "Category: {0}"),
                FText::FromString(CategoryInfo.Category));
        }
        else
        {
            CategoryLabel = FText::Format(
                LOCTEXT(
                    "CategoryBatchFmt",
                    "Category ({0} Categories)"),
                FText::AsNumber(CategoryCount));
        }

        AddScope(
            EHueStyleScope::Category,
            TEXT("HueCategory"),
            CategoryLabel,
            Nodes.Num() > 1
                ? FText::Format(
                    LOCTEXT(
                        "CategoryBatchTip",
                        "Style {0} unique Blueprint Category targets represented by the selected compatible nodes. Nodes without a Category are ignored."),
                    FText::AsNumber(CategoryCount))
                : LOCTEXT(
                    "CategoryTipSingle",
                    "Style the Blueprint Category represented by this node."));
    }
    else
    {
        Section.AddMenuEntry(
            TEXT("HueCategoryUnavailable"),
            LOCTEXT(
                "CategoryUnavailable",
                "Category: No Applicable Categories"),
            LOCTEXT(
                "CategoryUnavailableTip",
                "None of the selected compatible nodes represent a user-authored My Blueprint Category."),
            FSlateIcon(),
            FUIAction(
                FExecuteAction(),
                FCanExecuteAction::CreateLambda(
                    []() { return false; })));
    }

    const int32 GlobalCount =
        FHueStyleResolver::GetUniqueScopeTargetCount(
            Nodes,
            EHueStyleScope::GlobalFunction);

    if (GlobalCount > 0)
    {
        AddScope(
            EHueStyleScope::GlobalFunction,
            TEXT("HueGlobal"),
            HueMenuPrivate::ScopeLabel(
                Nodes,
                EHueStyleScope::GlobalFunction),
            Nodes.Num() > 1
                ? FText::Format(
                    LOCTEXT(
                        "GlobalBatchTip",
                        "Style {0} unique project-wide Hue targets represented by the current selection."),
                    FText::AsNumber(GlobalCount))
                : FHueStyleResolver::GetGlobalScopeTooltip(Nodes[0]));
    }
}

void FHueMenu::BuildScopeMenu(
    UToolMenu* Menu,
    TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes,
    EHueStyleScope Scope)
{
    const TArray<UEdGraphNode*> Nodes =
        HueMenuPrivate::GetLiveNodes(WeakNodes);

    if (!Menu || Nodes.IsEmpty())
    {
        return;
    }

    FToolMenuSection& Section =
        Menu->FindOrAddSection(TEXT("HueStyleChannels"));

    AddChannelEntries(
        Section,
        WeakNodes,
        Scope,
        EHueStyleChannel::HeaderColor,
        LOCTEXT("HeaderColor", "Header Color"),
        TEXT("HeaderColor"));

    AddChannelEntries(
        Section,
        WeakNodes,
        Scope,
        EHueStyleChannel::HeaderTextColor,
        LOCTEXT("HeaderTextColor", "Header Text Color"),
        TEXT("HeaderTextColor"));

    AddChannelEntries(
        Section,
        WeakNodes,
        Scope,
        EHueStyleChannel::BodyColor,
        LOCTEXT("BodyColor", "Body Color"),
        TEXT("BodyColor"));

    AddChannelEntries(
        Section,
        WeakNodes,
        Scope,
        EHueStyleChannel::BodyTextColor,
        LOCTEXT("BodyTextColor", "Body Text Color"),
        TEXT("BodyTextColor"));

    Section.AddSeparator(TEXT("HueClearSeparator"));

    Section.AddMenuEntry(
        TEXT("HueClearAll"),
        LOCTEXT("ClearAll", "Clear All Overrides"),
        FText::Format(
            LOCTEXT(
                "ClearAllTip",
                "Clear all {0} Hue overrides represented by the current selection."),
            HueMenuPrivate::ScopeLabel(Nodes, Scope)),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateLambda(
                [WeakNodes, Scope]()
                {
                    FHueStyleResolver::ClearAllOverrides(
                        HueMenuPrivate::GetLiveNodes(WeakNodes),
                        Scope);
                }),
            FCanExecuteAction::CreateLambda(
                [WeakNodes, Scope]()
                {
                    return HueMenuPrivate::HasAnyOverride(
                        HueMenuPrivate::GetLiveNodes(WeakNodes),
                        Scope);
                })));
}

void FHueMenu::AddChannelEntries(
    FToolMenuSection& Section,
    TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes,
    EHueStyleScope Scope,
    EHueStyleChannel Channel,
    const FText& Label,
    FName NamePrefix)
{
    const TArray<UEdGraphNode*> Nodes =
        HueMenuPrivate::GetLiveNodes(WeakNodes);

    UEdGraphNode* InitialNode = nullptr;
    for (UEdGraphNode* Node : Nodes)
    {
        if (HueMenuPrivate::IsScopeApplicable(Node, Scope))
        {
            InitialNode = Node;
            break;
        }
    }

    if (!InitialNode)
    {
        return;
    }

    const FText SetLabel = FText::Format(
        LOCTEXT("SetChannelFmt", "Set {0}..."),
        Label);

    const FText SetTooltip = Nodes.Num() > 1
        ? FText::Format(
            LOCTEXT(
                "SetChannelBatchTipFmt",
                "Choose the {0} for every applicable target represented by the selected nodes. Other Hue channels are left unchanged."),
            Label)
        : FText::Format(
            LOCTEXT(
                "SetChannelTipFmt",
                "Choose the {0} for this Hue scope. The swatch shows the value the picker will start from."),
            Label);

    const FLinearColor SwatchColor =
        FHueStyleResolver::GetPickerInitialColor(
            InitialNode,
            Scope,
            Channel);

    const FUIAction SetAction(
        FExecuteAction::CreateLambda(
            [WeakNodes, Scope, Channel]()
            {
                OpenHueColorPicker(
                    WeakNodes,
                    Scope,
                    Channel);
            }));

    FToolMenuEntry SetEntry =
        FToolMenuEntry::InitMenuEntry(
            HueMenuPrivate::MakeEntryName(
                NamePrefix,
                TEXT("Set")),
            FToolUIActionChoice(SetAction),
            HueMenuPrivate::MakeColorEntryWidget(
                SetLabel,
                SetTooltip,
                SwatchColor));

    SetEntry.ToolTip = SetTooltip;
    Section.AddEntry(SetEntry);

    Section.AddMenuEntry(
        HueMenuPrivate::MakeEntryName(
            NamePrefix,
            TEXT("Clear")),
        FText::Format(
            LOCTEXT("ClearChannelFmt", "Clear {0}"),
            Label),
        Nodes.Num() > 1
            ? FText::Format(
                LOCTEXT(
                    "ClearChannelBatchTipFmt",
                    "Remove this {0} override from every applicable target represented by the selected nodes."),
                Label)
            : FText::Format(
                LOCTEXT(
                    "ClearChannelTipFmt",
                    "Remove this {0} override and inherit the next available lower-precedence Hue value."),
                Label),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateLambda(
                [WeakNodes, Scope, Channel]()
                {
                    FHueStyleResolver::ClearColorOverrides(
                        HueMenuPrivate::GetLiveNodes(WeakNodes),
                        Scope,
                        Channel);
                }),
            FCanExecuteAction::CreateLambda(
                [WeakNodes, Scope, Channel]()
                {
                    return HueMenuPrivate::HasColorOverride(
                        HueMenuPrivate::GetLiveNodes(WeakNodes),
                        Scope,
                        Channel);
                })));
}

void FHueMenu::OpenHueColorPicker(
    TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes,
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    const TArray<UEdGraphNode*> Nodes =
        HueMenuPrivate::GetLiveNodes(WeakNodes);

    UEdGraphNode* InitialNode = nullptr;
    for (UEdGraphNode* Node : Nodes)
    {
        if (HueMenuPrivate::IsScopeApplicable(Node, Scope))
        {
            InitialNode = Node;
            break;
        }
    }

    if (!InitialNode)
    {
        return;
    }

    const FLinearColor InitialColor =
        FHueStyleResolver::GetPickerInitialColor(
            InitialNode,
            Scope,
            Channel);

    FColorPickerArgs PickerArgs(
        InitialColor,
        FOnLinearColorValueChanged::CreateLambda(
            [WeakNodes, Scope, Channel](FLinearColor NewColor)
            {
                FHueStyleResolver::SetColorOverrides(
                    HueMenuPrivate::GetLiveNodes(WeakNodes),
                    Scope,
                    Channel,
                    NewColor);
            }));

    PickerArgs.bUseAlpha = false;
    PickerArgs.bClampValue = true;
    PickerArgs.bOnlyRefreshOnOk = true;
    ::OpenColorPicker(PickerArgs);
}

#undef LOCTEXT_NAMESPACE
