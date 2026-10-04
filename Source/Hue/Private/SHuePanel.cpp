// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHuePanel.h"

#include "BlueprintEditor.h"
#include "EdGraph/EdGraphNode.h"
#include "Engine/Blueprint.h"
#include "HueStyleResolver.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"
#include "Styling/AppStyle.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "HuePanel"

void SHuePanel::Construct(const FArguments& InArgs)
{
    BlueprintEditorPtr = InArgs._BlueprintEditor;

    ChildSlot
    [
        SAssignNew(RootBox, SVerticalBox)
    ];

    RebuildPanel();
}

void SHuePanel::Tick(
    const FGeometry& AllottedGeometry,
    const double InCurrentTime,
    const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

    UEdGraphNode* NewNode = GetSelectedNode();

    FHueBlueprintCategoryInfo CurrentCategoryInfo;
    const bool bCurrentCategoryAvailable =
        FHueStyleResolver::GetBlueprintCategoryInfo(NewNode, CurrentCategoryInfo);

    const FString CurrentCategory = bCurrentCategoryAvailable
        ? CurrentCategoryInfo.Category
        : FString();

    UBlueprint* CurrentCategoryBlueprint = bCurrentCategoryAvailable
        ? CurrentCategoryInfo.DefiningBlueprint
        : nullptr;

    const bool bSelectionChanged = SelectedNode.Get() != NewNode;
    const bool bCategoryChanged =
        bCachedCategoryAvailable != bCurrentCategoryAvailable
        || CachedCategory != CurrentCategory
        || CachedCategoryBlueprint.Get() != CurrentCategoryBlueprint;

    if (bSelectionChanged || bCategoryChanged)
    {
        SelectedNode = NewNode;
        bCachedCategoryAvailable = bCurrentCategoryAvailable;
        CachedCategory = CurrentCategory;
        CachedCategoryBlueprint = CurrentCategoryBlueprint;
        RebuildPanel();
    }
}

UEdGraphNode* SHuePanel::GetSelectedNode() const
{
    TSharedPtr<FBlueprintEditor> BlueprintEditor = BlueprintEditorPtr.Pin();
    if (!BlueprintEditor.IsValid())
    {
        return nullptr;
    }

    UEdGraphNode* Node = BlueprintEditor->GetSingleSelectedNode();
    return FHueStyleResolver::IsSupportedNode(Node) ? Node : nullptr;
}

void SHuePanel::RebuildPanel()
{
    if (!RootBox.IsValid())
    {
        return;
    }

    RootBox->ClearChildren();

    UEdGraphNode* Node = SelectedNode.Get();
    if (!Node)
    {
        RootBox->AddSlot()
        .AutoHeight()
        .Padding(10.0f)
        [
            SNew(STextBlock)
            .Text(LOCTEXT(
                "SelectSupportedNode",
                "Select a supported Blueprint node to edit its Hue style."))
            .ToolTipText(LOCTEXT(
                "SelectSupportedNodeTooltip",
                "Hue supports standard and compact K2 nodes plus compatible specialized presentations such as variables, math operators, Switch, Sequence, Timeline, Events, Format Text, collapsed graphs, Create Event, Spawn Actor, and other compatible native-specialized nodes. Legacy Documentation nodes are intentionally unsupported. Some node-owned custom widgets still require dedicated compatibility work."))
            .AutoWrapText(true)
            .ColorAndOpacity(FSlateColor::UseSubduedForeground())
        ];
        return;
    }

    RootBox->AddSlot()
    .AutoHeight()
    .Padding(10.0f, 8.0f, 10.0f, 8.0f)
    [
        SNew(STextBlock)
        .Text(Node->GetNodeTitle(ENodeTitleType::ListView))
        .Font(FAppStyle::GetFontStyle("BoldFont"))
        .ToolTipText(LOCTEXT(
            "SelectedNodeTooltip",
            "Hue is editing visual style overrides for this selected Blueprint node."))
    ];

    RootBox->AddSlot()
    .FillHeight(1.0f)
    [
        SNew(SScrollBox)

        + SScrollBox::Slot()
        .Padding(8.0f, 2.0f)
        [
            BuildScopeSection(EHueStyleScope::Instance)
        ]

        + SScrollBox::Slot()
        .Padding(8.0f, 2.0f)
        [
            BuildScopeSection(EHueStyleScope::Category)
        ]

        + SScrollBox::Slot()
        .Padding(8.0f, 2.0f)
        [
            BuildScopeSection(EHueStyleScope::GlobalFunction)
        ]
    ];
}

TSharedRef<SWidget> SHuePanel::BuildScopeSection(EHueStyleScope Scope)
{
    UEdGraphNode* Node = SelectedNode.Get();

    FHueBlueprintCategoryInfo CategoryInfo;
    const bool bCategoryAvailable =
        Scope != EHueStyleScope::Category
        || FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo);

    TSharedRef<SHorizontalBox> HeaderRow = SNew(SHorizontalBox);

    HeaderRow->AddSlot()
    .FillWidth(1.0f)
    .VAlign(VAlign_Center)
    [
        SNew(SButton)
        .ButtonStyle(FAppStyle::Get(), "NoBorder")
        .ContentPadding(FMargin(2.0f, 2.0f))
        .ToolTipText(GetScopeTooltip(Scope))
        .OnClicked(this, &SHuePanel::OnScopeHeaderClicked, Scope)
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            .Padding(0.0f, 0.0f, 4.0f, 0.0f)
            [
                SNew(SImage)
                .Image_Lambda([this, Scope]()
                {
                    return GetScopeExpansionBrush(Scope);
                })
            ]

            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Text(GetScopeLabel(Scope))
                .Font(FAppStyle::GetFontStyle("BoldFont"))
            ]

            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            .Padding(6.0f, 0.0f, 0.0f, 0.0f)
            [
                SNew(SImage)
                .Image(FAppStyle::GetBrush("Icons.Warning"))
                .ColorAndOpacity(FSlateColor::UseSubduedForeground())
                .ToolTipText(LOCTEXT(
                    "CategoryWarningTooltip",
                    "This node has no applicable My Blueprint Category assigned, so shared Category styling is unavailable."))
                .Visibility(
                    Scope == EHueStyleScope::Category && !bCategoryAvailable
                        ? EVisibility::Visible
                        : EVisibility::Collapsed)
            ]
        ]
    ];

    if (Scope == EHueStyleScope::GlobalFunction)
    {
        HeaderRow->AddSlot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        .Padding(2.0f, 0.0f, 2.0f, 0.0f)
        [
            SNew(SButton)
            .ButtonStyle(FAppStyle::Get(), "HoverHintOnly")
            .ContentPadding(3.0f)
            .ForegroundColor(FSlateColor::UseForeground())
            .ToolTipText(LOCTEXT(
                "OpenHueSettingsTooltip",
                "Open Project Settings > Plugins > Hue."))
            .OnClicked(this, &SHuePanel::OnOpenHueSettingsClicked)
            [
                SNew(SImage)
                .Image(FAppStyle::GetBrush("Icons.Settings"))
            ]
        ];
    }

    TSharedRef<SVerticalBox> Content = SNew(SVerticalBox);

    if (Scope == EHueStyleScope::Category)
    {
        if (bCategoryAvailable)
        {
            Content->AddSlot()
            .AutoHeight()
            .Padding(6.0f, 5.0f, 6.0f, 1.0f)
            [
                SNew(STextBlock)
                .Text(FText::Format(
                    LOCTEXT("CategoryName", "Category: {0}"),
                    FText::FromString(CategoryInfo.Category)))
                .ToolTipText(LOCTEXT(
                    "CategoryNameTooltip",
                    "This is the user-assigned My Blueprint Category on the member definition. Supported functions, macros, Blueprint-authored events, and Blueprint member variables in the same Category share these Category overrides."))
            ];

            Content->AddSlot()
            .AutoHeight()
            .Padding(6.0f, 0.0f, 6.0f, 5.0f)
            [
                SNew(STextBlock)
                .Text(FText::Format(
                    LOCTEXT("CategoryOwner", "Defined by: {0}"),
                    FText::FromString(
                        CategoryInfo.DefiningBlueprint
                            ? CategoryInfo.DefiningBlueprint->GetName()
                            : FString())))
                .ToolTipText(LOCTEXT(
                    "CategoryOwnerTooltip",
                    "Category ownership follows the Blueprint that defines the function, macro, event, or member variable."))
                .ColorAndOpacity(FSlateColor::UseSubduedForeground())
            ];
        }
        else
        {
            UBlueprint* DefiningBlueprint = FHueStyleResolver::GetDefiningBlueprint(Node);

            Content->AddSlot()
            .AutoHeight()
            .Padding(6.0f, 5.0f, 6.0f, 1.0f)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("NoCategoryAssigned", "No Category Assigned"))
                .ToolTipText(LOCTEXT(
                    "NoCategoryAssignedTooltip",
                    "Hue Category styling becomes available when a supported Blueprint-defined function, macro, event, or member variable has an explicit My Blueprint Category."))
                .ColorAndOpacity(FSlateColor::UseSubduedForeground())
            ];

            Content->AddSlot()
            .AutoHeight()
            .Padding(6.0f, 0.0f, 6.0f, 5.0f)
            [
                SNew(STextBlock)
                .Text(DefiningBlueprint
                    ? LOCTEXT(
                        "AssignCategoryHelp",
                        "Assign a Category to this function, macro, event, or member variable in its Details panel to enable shared Category styling.")
                    : LOCTEXT(
                        "NoCategoryForNodeTypeHelp",
                        "Category styling applies to categorized Blueprint-defined functions, macros, events, and member variables. This node type has no My Blueprint member Category."))
                .ToolTipText(DefiningBlueprint
                    ? LOCTEXT(
                        "AssignCategoryHelpTooltip",
                        "Select the function, macro, event, or member variable in My Blueprint, then assign its Category in the Details panel.")
                    : LOCTEXT(
                        "NoCategoryForNodeTypeHelpTooltip",
                        "Utility nodes and native members are not part of a user-authored My Blueprint Category unless they resolve to a Blueprint-authored categorized member."))
                .AutoWrapText(true)
                .ColorAndOpacity(FSlateColor::UseSubduedForeground())
            ];
        }
    }

    const EHueStyleChannel Channels[] =
    {
        EHueStyleChannel::HeaderColor,
        EHueStyleChannel::HeaderTextColor,
        EHueStyleChannel::BodyColor,
        EHueStyleChannel::BodyTextColor
    };

    for (int32 ChannelIndex = 0; ChannelIndex < UE_ARRAY_COUNT(Channels); ++ChannelIndex)
    {
        Content->AddSlot()
        .AutoHeight()
        [
            BuildChannelRow(Scope, Channels[ChannelIndex], bCategoryAvailable)
        ];

        if (ChannelIndex < UE_ARRAY_COUNT(Channels) - 1)
        {
            Content->AddSlot()
            .AutoHeight()
            .Padding(6.0f, 0.0f)
            [
                SNew(SSeparator)
                .Orientation(Orient_Horizontal)
            ];
        }
    }

    Content->AddSlot()
    .AutoHeight()
    .Padding(6.0f, 2.0f, 6.0f, 0.0f)
    [
        SNew(SSeparator)
        .Orientation(Orient_Horizontal)
    ];

    Content->AddSlot()
    .AutoHeight()
    .HAlign(HAlign_Right)
    .Padding(4.0f, 5.0f, 4.0f, 5.0f)
    [
        SNew(SButton)
        .Text(LOCTEXT("ClearScope", "Clear All"))
        .ToolTipText(FText::Format(
            LOCTEXT("ClearScopeTooltip", "Clear every {0} Hue override in this scope."),
            GetScopeLabel(Scope)))
        .IsEnabled_Lambda([this, Scope, bCategoryAvailable]()
        {
            if (!bCategoryAvailable)
            {
                return false;
            }

            UEdGraphNode* LiveNode = SelectedNode.Get();
            return LiveNode && FHueStyleResolver::HasAnyOverride(LiveNode, Scope);
        })
        .OnClicked(this, &SHuePanel::OnClearScopeClicked, Scope)
    ];

    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SBorder)
            .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
            .Padding(FMargin(2.0f, 1.0f))
            [
                HeaderRow
            ]
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SBorder)
            .BorderImage(FAppStyle::GetBrush("NoBorder"))
            .Padding(FMargin(2.0f, 2.0f, 2.0f, 5.0f))
            .Visibility_Lambda([this, Scope]()
            {
                return IsScopeExpanded(Scope)
                    ? EVisibility::Visible
                    : EVisibility::Collapsed;
            })
            [
                Content
            ]
        ];
}

TSharedRef<SWidget> SHuePanel::BuildChannelRow(
    EHueStyleScope Scope,
    EHueStyleChannel Channel,
    bool bScopeEnabled)
{
    const FText ChannelTooltip = GetChannelTooltip(Channel);

    return SNew(SHorizontalBox)
        .IsEnabled(bScopeEnabled)
        + SHorizontalBox::Slot()
        .FillWidth(1.0f)
        .VAlign(VAlign_Center)
        .Padding(6.0f, 3.0f)
        [
            SNew(STextBlock)
            .Text(GetChannelLabel(Channel))
            .ToolTipText(ChannelTooltip)
        ]
        + SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        .Padding(4.0f, 3.0f)
        [
            SNew(STextBlock)
            .Text_Lambda([this, Scope, Channel]()
            {
                return GetOverrideStateText(Scope, Channel);
            })
            .ToolTipText_Lambda([this, Scope, Channel]()
            {
                return GetOverrideStateTooltip(Scope, Channel);
            })
            .ColorAndOpacity(FSlateColor::UseSubduedForeground())
        ]
        + SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        .Padding(2.0f, 2.0f)
        [
            SNew(SButton)
            .ContentPadding(2.0f)
            .ToolTipText(FText::Format(
                LOCTEXT("SetChannelTooltip", "Choose {0} for the {1} scope. {2}"),
                GetChannelLabel(Channel),
                GetScopeLabel(Scope),
                ChannelTooltip))
            .OnClicked(this, &SHuePanel::OnSetColorClicked, Scope, Channel)
            [
                SNew(SColorBlock)
                .Color_Lambda([this, Scope, Channel]()
                {
                    return GetSwatchColor(Scope, Channel);
                })
                .Size(FVector2D(38.0f, 16.0f))
                .ShowBackgroundForAlpha(false)
            ]
        ]
        + SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        .Padding(2.0f, 2.0f, 4.0f, 2.0f)
        [
            SNew(SButton)
            .Text(LOCTEXT("ClearChannel", "Clear"))
            .ToolTipText(FText::Format(
                LOCTEXT("ClearChannelTooltip", "Clear this {0} override. Hue will inherit the next available lower-precedence value."),
                GetChannelLabel(Channel)))
            .IsEnabled_Lambda([this, Scope, Channel, bScopeEnabled]()
            {
                if (!bScopeEnabled)
                {
                    return false;
                }

                UEdGraphNode* LiveNode = SelectedNode.Get();
                return LiveNode && FHueStyleResolver::HasColorOverride(LiveNode, Scope, Channel);
            })
            .OnClicked(this, &SHuePanel::OnClearColorClicked, Scope, Channel)
        ];
}

FText SHuePanel::GetScopeLabel(EHueStyleScope Scope) const
{
    switch (Scope)
    {
    case EHueStyleScope::Instance:
        return LOCTEXT("InstanceScope", "Instance");
    case EHueStyleScope::Category:
        return LOCTEXT("CategoryScope", "Category");
    case EHueStyleScope::GlobalFunction:
        return FHueStyleResolver::GetGlobalScopeLabel(SelectedNode.Get());
    }
    return FText::GetEmpty();
}

FText SHuePanel::GetScopeTooltip(EHueStyleScope Scope) const
{
    switch (Scope)
    {
    case EHueStyleScope::Instance:
        return LOCTEXT(
            "InstanceScopeTooltip",
            "Style only this selected node instance. Instance overrides have the highest Hue precedence. Click to collapse or expand this section.");

    case EHueStyleScope::Category:
    {
        UEdGraphNode* Node = SelectedNode.Get();
        FHueBlueprintCategoryInfo CategoryInfo;
        if (FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return FText::Format(
                LOCTEXT(
                    "CategoryScopeTooltipAvailable",
                    "Shared style for supported Blueprint members in the My Blueprint Category '{0}' defined by {1}. Click to collapse or expand this section."),
                FText::FromString(CategoryInfo.Category),
                FText::FromString(
                    CategoryInfo.DefiningBlueprint
                        ? CategoryInfo.DefiningBlueprint->GetName()
                        : FString()));
        }

        return LOCTEXT(
            "CategoryScopeTooltipUnavailable",
            "Category styling is unavailable because this node has no applicable user-assigned My Blueprint Category. Click to collapse or expand this section.");
    }

    case EHueStyleScope::GlobalFunction:
        return FText::Format(
            LOCTEXT("GlobalScopeTooltipFmt", "{0} Click to collapse or expand this section."),
            FHueStyleResolver::GetGlobalScopeTooltip(SelectedNode.Get()));
    }

    return FText::GetEmpty();
}

FText SHuePanel::GetChannelLabel(EHueStyleChannel Channel)
{
    switch (Channel)
    {
    case EHueStyleChannel::HeaderColor:
        return LOCTEXT("HeaderColor", "Header Color");
    case EHueStyleChannel::HeaderTextColor:
        return LOCTEXT("HeaderTextColor", "Header Text Color");
    case EHueStyleChannel::BodyColor:
        return LOCTEXT("BodyColor", "Body Color");
    case EHueStyleChannel::BodyTextColor:
        return LOCTEXT("BodyTextColor", "Body Text Color");
    }
    return FText::GetEmpty();
}

FText SHuePanel::GetChannelTooltip(EHueStyleChannel Channel)
{
    switch (Channel)
    {
    case EHueStyleChannel::HeaderColor:
        return LOCTEXT("HeaderColorTooltip", "Controls the colored title/header area. On compact nodes with one central visual fill, Header Color controls that compact fill.");
    case EHueStyleChannel::HeaderTextColor:
        return LOCTEXT(
            "HeaderTextColorTooltip",
            "Controls the main node title and secondary title lines such as 'Target is ...'. Secondary lines keep Unreal's native subdued opacity.");
    case EHueStyleChannel::BodyColor:
        return LOCTEXT("BodyColorTooltip", "Controls the main body tint. On compact nodes, Body Color is used as the compact fill fallback when Header Color has no Hue override.");
    case EHueStyleChannel::BodyTextColor:
        return LOCTEXT(
            "BodyTextColorTooltip",
            "Controls pin-label text and execution-pin triangle color. Editable value controls keep Unreal's native styling.");
    }
    return FText::GetEmpty();
}

FText SHuePanel::GetOverrideStateText(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    UEdGraphNode* Node = SelectedNode.Get();
    if (!Node)
    {
        return FText::GetEmpty();
    }

    if (Scope == EHueStyleScope::Category)
    {
        FHueBlueprintCategoryInfo CategoryInfo;
        if (!FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return LOCTEXT("UnavailableState", "Unavailable");
        }
    }

    return FHueStyleResolver::HasColorOverride(Node, Scope, Channel)
        ? LOCTEXT("OverrideState", "Override")
        : LOCTEXT("InheritedState", "Inherited");
}

FText SHuePanel::GetInheritedSourceTooltip(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    UEdGraphNode* Node = SelectedNode.Get();
    if (!Node)
    {
        return FText::GetEmpty();
    }

    if (Scope == EHueStyleScope::Instance)
    {
        FHueBlueprintCategoryInfo CategoryInfo;
        if (FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo)
            && FHueStyleResolver::HasColorOverride(Node, EHueStyleScope::Category, Channel))
        {
            return FText::Format(
                LOCTEXT(
                    "InheritedFromCategoryFmt",
                    "Inherited from Category '{0}' defined by {1}. Category has the next active value below Instance for this channel."),
                FText::FromString(CategoryInfo.Category),
                FText::FromString(
                    CategoryInfo.DefiningBlueprint
                        ? CategoryInfo.DefiningBlueprint->GetName()
                        : FString()));
        }

        if (FHueStyleResolver::HasColorOverride(Node, EHueStyleScope::GlobalFunction, Channel))
        {
            return FText::Format(
                LOCTEXT("InheritedFromGlobalFmt", "Inherited from {0}. No higher-precedence Category value is active for this channel."),
                FHueStyleResolver::GetGlobalScopeLabel(Node));
        }

        return LOCTEXT(
            "InheritedFromNativeInstance",
            "Inherited from Unreal Default. Neither Category nor the applicable Global scope overrides this channel.");
    }

    if (Scope == EHueStyleScope::Category)
    {
        if (FHueStyleResolver::HasColorOverride(Node, EHueStyleScope::GlobalFunction, Channel))
        {
            return FText::Format(
                LOCTEXT("CategoryInheritedFromGlobalFmt", "Inherited from {0}. This Category has no override for the channel."),
                FHueStyleResolver::GetGlobalScopeLabel(Node));
        }

        return LOCTEXT(
            "CategoryInheritedFromNative",
            "Inherited from Unreal Default. Neither this Category nor the applicable Global scope overrides this channel.");
    }

    return LOCTEXT(
        "GlobalInheritedFromNative",
        "Inherited from Unreal Default. No project-wide Hue override is set for this channel.");
}

FText SHuePanel::GetOverrideStateTooltip(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    UEdGraphNode* Node = SelectedNode.Get();
    if (!Node)
    {
        return FText::GetEmpty();
    }

    if (Scope == EHueStyleScope::Category)
    {
        FHueBlueprintCategoryInfo CategoryInfo;
        if (!FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return LOCTEXT(
                "UnavailableStateTooltip",
                "No applicable My Blueprint Category is assigned, so this Category channel cannot provide a Hue value.");
        }
    }

    if (FHueStyleResolver::HasColorOverride(Node, Scope, Channel))
    {
        return FText::Format(
            LOCTEXT("OverrideStateTooltip", "The {0} scope explicitly overrides this channel."),
            GetScopeLabel(Scope));
    }

    return GetInheritedSourceTooltip(Scope, Channel);
}

FLinearColor SHuePanel::GetSwatchColor(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    if (UEdGraphNode* Node = SelectedNode.Get())
    {
        return FHueStyleResolver::GetPickerInitialColor(Node, Scope, Channel);
    }
    return FLinearColor::White;
}

bool SHuePanel::IsScopeExpanded(EHueStyleScope Scope) const
{
    switch (Scope)
    {
    case EHueStyleScope::Instance:
        return bInstanceExpanded;
    case EHueStyleScope::Category:
        return bCategoryExpanded;
    case EHueStyleScope::GlobalFunction:
        return bGlobalExpanded;
    }
    return true;
}

const FSlateBrush* SHuePanel::GetScopeExpansionBrush(EHueStyleScope Scope) const
{
    return FAppStyle::GetBrush(
        IsScopeExpanded(Scope)
            ? "TreeArrow_Expanded"
            : "TreeArrow_Collapsed");
}

FReply SHuePanel::OnScopeHeaderClicked(EHueStyleScope Scope)
{
    switch (Scope)
    {
    case EHueStyleScope::Instance:
        bInstanceExpanded = !bInstanceExpanded;
        break;
    case EHueStyleScope::Category:
        bCategoryExpanded = !bCategoryExpanded;
        break;
    case EHueStyleScope::GlobalFunction:
        bGlobalExpanded = !bGlobalExpanded;
        break;
    }
    return FReply::Handled();
}

FReply SHuePanel::OnSetColorClicked(
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    TWeakObjectPtr<UEdGraphNode> WeakNode = SelectedNode;
    UEdGraphNode* Node = WeakNode.Get();
    if (!Node)
    {
        return FReply::Handled();
    }

    if (Scope == EHueStyleScope::Category)
    {
        FHueBlueprintCategoryInfo CategoryInfo;
        if (!FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return FReply::Handled();
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

    return FReply::Handled();
}

FReply SHuePanel::OnClearColorClicked(
    EHueStyleScope Scope,
    EHueStyleChannel Channel)
{
    if (UEdGraphNode* Node = SelectedNode.Get())
    {
        FHueStyleResolver::ClearColorOverride(Node, Scope, Channel);
    }
    return FReply::Handled();
}

FReply SHuePanel::OnClearScopeClicked(EHueStyleScope Scope)
{
    if (UEdGraphNode* Node = SelectedNode.Get())
    {
        FHueStyleResolver::ClearAllOverrides(Node, Scope);
    }
    return FReply::Handled();
}

FReply SHuePanel::OnOpenHueSettingsClicked()
{
    if (ISettingsModule* SettingsModule =
        FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->ShowViewer("Project", "Plugins", "Hue");
    }
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
