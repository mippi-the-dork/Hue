// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueMenu.h"

#include "EdGraph/EdGraphNode.h"
#include "Engine/Blueprint.h"
#include "HueStyleResolver.h"
#include "Styling/AppStyle.h"
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
    static FText ScopeLabel(const UEdGraphNode* Node, EHueStyleScope Scope)
    {
        switch (Scope)
        {
        case EHueStyleScope::Instance:
            return LOCTEXT("InstanceScope", "Instance");
        case EHueStyleScope::Category:
            return LOCTEXT("CategoryScope", "Category");
        case EHueStyleScope::GlobalFunction:
            return FHueStyleResolver::GetGlobalScopeLabel(Node);
        }
        return FText::GetEmpty();
    }

    static FName MakeEntryName(FName Prefix, const TCHAR* Suffix)
    {
        return FName(*(Prefix.ToString() + Suffix));
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

void FHueMenu::BuildNodeContextEntry(FToolMenuSection& Section)
{
    UGraphNodeContextMenuContext* Context =
        Section.FindContext<UGraphNodeContextMenuContext>();

    if (!Context || Context->bIsDebugging)
    {
        return;
    }

    const UEdGraphNode* ConstNode = Context->Node;
    if (!FHueStyleResolver::IsSupportedNode(ConstNode))
    {
        return;
    }

    TWeakObjectPtr<UEdGraphNode> WeakNode(const_cast<UEdGraphNode*>(ConstNode));

    Section.AddSubMenu(
        TEXT("Hue"),
        LOCTEXT("HueSubMenu", "Hue"),
        LOCTEXT("HueSubMenuTooltip", "Set visual style overrides for this Blueprint node."),
        FNewToolMenuChoice(
            FNewToolMenuDelegate::CreateLambda(
                [WeakNode](UToolMenu* Menu)
                {
                    BuildHueMenu(Menu, WeakNode);
                })),
        false,
        FSlateIcon(),
        true,
        NAME_None);
}

void FHueMenu::BuildHueMenu(
    UToolMenu* Menu,
    TWeakObjectPtr<UEdGraphNode> WeakNode)
{
    UEdGraphNode* Node = WeakNode.Get();
    if (!Menu || !Node)
    {
        return;
    }

    FToolMenuSection& Section = Menu->FindOrAddSection(TEXT("HueScopes"));

    auto AddScope = [&Section, WeakNode](
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
                    [WeakNode, Scope](UToolMenu* ScopeMenu)
                    {
                        BuildScopeMenu(ScopeMenu, WeakNode, Scope);
                    })),
            false,
            FSlateIcon(),
            true,
            NAME_None);
    };

    AddScope(
        EHueStyleScope::Instance,
        TEXT("HueInstance"),
        LOCTEXT("Instance", "Instance"),
        LOCTEXT("InstanceTip", "Style only this node instance."));

    FHueBlueprintCategoryInfo CategoryInfo;
    if (FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
    {
        AddScope(
            EHueStyleScope::Category,
            TEXT("HueCategory"),
            FText::Format(
                LOCTEXT("CategoryFmt", "Category: {0}"),
                FText::FromString(CategoryInfo.Category)),
            FText::Format(
                LOCTEXT(
                    "CategoryTip",
                    "Style supported Blueprint members in the {0} category defined by {1}."),
                FText::FromString(CategoryInfo.Category),
                FText::FromString(
                    CategoryInfo.DefiningBlueprint
                        ? CategoryInfo.DefiningBlueprint->GetName()
                        : FString())));
    }
    else
    {
        const bool bHasDefiningBlueprint =
            FHueStyleResolver::GetDefiningBlueprint(Node) != nullptr;

        Section.AddMenuEntry(
            TEXT("HueCategoryUnavailable"),
            LOCTEXT("CategoryUnavailable", "Category: No Category Assigned"),
            bHasDefiningBlueprint
                ? LOCTEXT(
                    "CategoryUnavailableTip",
                    "Assign a Category to this Blueprint function, macro, event, or member variable in its Details panel to enable shared Category styling.")
                : LOCTEXT(
                    "NodeCategoryUnavailableTip",
                    "This node type does not represent a user-authored My Blueprint member Category."),
            FSlateIcon(),
            FUIAction(
                FExecuteAction(),
                FCanExecuteAction::CreateLambda([]() { return false; })));
    }

    AddScope(
        EHueStyleScope::GlobalFunction,
        TEXT("HueGlobal"),
        FHueStyleResolver::GetGlobalScopeLabel(Node),
        FHueStyleResolver::GetGlobalScopeTooltip(Node));
}

void FHueMenu::BuildScopeMenu(
    UToolMenu* Menu,
    TWeakObjectPtr<UEdGraphNode> WeakNode,
    EHueStyleScope Scope)
{
    if (!Menu || !WeakNode.IsValid())
    {
        return;
    }

    FToolMenuSection& Section = Menu->FindOrAddSection(TEXT("HueStyleChannels"));

    AddChannelEntries(Section, WeakNode, Scope, EHueStyleChannel::HeaderColor, LOCTEXT("HeaderColor", "Header Color"), TEXT("HeaderColor"));
    AddChannelEntries(Section, WeakNode, Scope, EHueStyleChannel::HeaderTextColor, LOCTEXT("HeaderTextColor", "Header Text Color"), TEXT("HeaderTextColor"));
    AddChannelEntries(Section, WeakNode, Scope, EHueStyleChannel::BodyColor, LOCTEXT("BodyColor", "Body Color"), TEXT("BodyColor"));
    AddChannelEntries(Section, WeakNode, Scope, EHueStyleChannel::BodyTextColor, LOCTEXT("BodyTextColor", "Body Text Color"), TEXT("BodyTextColor"));

    Section.AddSeparator(TEXT("HueClearSeparator"));

    Section.AddMenuEntry(
        TEXT("HueClearAll"),
        LOCTEXT("ClearAll", "Clear All Overrides"),
        FText::Format(
            LOCTEXT("ClearAllTip", "Clear all {0} Hue overrides for this node context."),
            HueMenuPrivate::ScopeLabel(WeakNode.Get(), Scope)),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateLambda([WeakNode, Scope]()
            {
                if (UEdGraphNode* Node = WeakNode.Get())
                {
                    FHueStyleResolver::ClearAllOverrides(Node, Scope);
                }
            }),
            FCanExecuteAction::CreateLambda([WeakNode, Scope]()
            {
                return WeakNode.IsValid()
                    && FHueStyleResolver::HasAnyOverride(WeakNode.Get(), Scope);
            })));
}

void FHueMenu::AddChannelEntries(
    FToolMenuSection& Section,
    TWeakObjectPtr<UEdGraphNode> WeakNode,
    EHueStyleScope Scope,
    EHueStyleChannel Channel,
    const FText& Label,
    FName NamePrefix)
{
    UEdGraphNode* Node = WeakNode.Get();
    if (!Node)
    {
        return;
    }

    const FText SetLabel = FText::Format(
        LOCTEXT("SetChannelFmt", "Set {0}..."),
        Label);
    const FText SetTooltip = FText::Format(
        LOCTEXT("SetChannelTipFmt", "Choose the {0} for this Hue scope. The swatch shows the value the picker will start from."),
        Label);
    const FLinearColor SwatchColor =
        FHueStyleResolver::GetPickerInitialColor(Node, Scope, Channel);

    const FUIAction SetAction(
        FExecuteAction::CreateLambda([WeakNode, Scope, Channel]()
        {
            OpenHueColorPicker(WeakNode, Scope, Channel);
        }));

    FToolMenuEntry SetEntry = FToolMenuEntry::InitMenuEntry(
        HueMenuPrivate::MakeEntryName(NamePrefix, TEXT("Set")),
        FToolUIActionChoice(SetAction),
        HueMenuPrivate::MakeColorEntryWidget(SetLabel, SetTooltip, SwatchColor));
    SetEntry.ToolTip = SetTooltip;
    Section.AddEntry(SetEntry);

    Section.AddMenuEntry(
        HueMenuPrivate::MakeEntryName(NamePrefix, TEXT("Clear")),
        FText::Format(LOCTEXT("ClearChannelFmt", "Clear {0}"), Label),
        FText::Format(
            LOCTEXT("ClearChannelTipFmt", "Remove this {0} override and inherit the next available lower-precedence Hue value."),
            Label),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateLambda([WeakNode, Scope, Channel]()
            {
                if (UEdGraphNode* LiveNode = WeakNode.Get())
                {
                    FHueStyleResolver::ClearColorOverride(LiveNode, Scope, Channel);
                }
            }),
            FCanExecuteAction::CreateLambda([WeakNode, Scope, Channel]()
            {
                return WeakNode.IsValid()
                    && FHueStyleResolver::HasColorOverride(WeakNode.Get(), Scope, Channel);
            })));
}

void FHueMenu::OpenHueColorPicker(
    TWeakObjectPtr<UEdGraphNode> WeakNode,
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    UEdGraphNode* Node = WeakNode.Get();
    if (!Node)
    {
        return;
    }

    if (Scope == EHueStyleScope::Category)
    {
        FHueBlueprintCategoryInfo CategoryInfo;
        if (!FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return;
        }
    }

    const FLinearColor InitialColor =
        FHueStyleResolver::GetPickerInitialColor(Node, Scope, Channel);

    FColorPickerArgs PickerArgs(
        InitialColor,
        FOnLinearColorValueChanged::CreateLambda(
            [WeakNode, Scope, Channel](FLinearColor NewColor)
            {
                if (UEdGraphNode* LiveNode = WeakNode.Get())
                {
                    FHueStyleResolver::SetColorOverride(LiveNode, Scope, Channel, NewColor);
                }
            }));

    PickerArgs.bUseAlpha = false;
    PickerArgs.bClampValue = true;
    PickerArgs.bOnlyRefreshOnOk = true;
    ::OpenColorPicker(PickerArgs);
}

#undef LOCTEXT_NAMESPACE
