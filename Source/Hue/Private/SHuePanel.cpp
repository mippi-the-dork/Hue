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

namespace HuePanelPrivate
{
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

    static bool NearlyEqualColor(
        const FLinearColor& A,
        const FLinearColor& B)
    {
        return A.Equals(B, KINDA_SMALL_NUMBER);
    }
}

void SHuePanel::Construct(const FArguments& InArgs)
{
    BlueprintEditorPtr = InArgs._BlueprintEditor;

    ChildSlot
    [
        SAssignNew(RootBox, SVerticalBox)
    ];

    RefreshSelectionCache();
    RebuildPanel();
}

void SHuePanel::Tick(
    const FGeometry& AllottedGeometry,
    const double InCurrentTime,
    const float InDeltaTime)
{
    SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

    TArray<UEdGraphNode*> NewSupportedNodes;
    int32 NewTotalNodeCount = 0;
    GatherSelection(NewSupportedNodes, NewTotalNodeCount);

    const FString NewKey =
        BuildSelectionKey(NewSupportedNodes, NewTotalNodeCount);

    if (NewKey != CachedSelectionKey)
    {
        SelectedNodes.Reset();
        for (UEdGraphNode* Node : NewSupportedNodes)
        {
            SelectedNodes.Add(Node);
        }

        TotalSelectedNodeCount = NewTotalNodeCount;
        CachedSelectionKey = NewKey;
        RebuildPanel();
    }
}

void SHuePanel::GatherSelection(
    TArray<UEdGraphNode*>& OutSupportedNodes,
    int32& OutTotalNodeCount) const
{
    OutSupportedNodes.Reset();
    OutTotalNodeCount = 0;

    const TSharedPtr<FBlueprintEditor> BlueprintEditor =
        BlueprintEditorPtr.Pin();

    if (!BlueprintEditor.IsValid())
    {
        return;
    }

    const FGraphPanelSelectionSet Selection =
        BlueprintEditor->GetSelectedNodes();

    for (UObject* SelectedObject : Selection)
    {
        UEdGraphNode* Node = Cast<UEdGraphNode>(SelectedObject);
        if (!Node)
        {
            continue;
        }

        ++OutTotalNodeCount;

        if (FHueStyleResolver::IsSupportedNode(Node))
        {
            OutSupportedNodes.Add(Node);
        }
    }

}

TArray<UEdGraphNode*> SHuePanel::GetLiveSelectedNodes() const
{
    TArray<UEdGraphNode*> Result;
    Result.Reserve(SelectedNodes.Num());

    for (const TWeakObjectPtr<UEdGraphNode>& WeakNode : SelectedNodes)
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

UEdGraphNode* SHuePanel::GetPrimaryNode() const
{
    for (const TWeakObjectPtr<UEdGraphNode>& WeakNode : SelectedNodes)
    {
        if (UEdGraphNode* Node = WeakNode.Get())
        {
            return Node;
        }
    }
    return nullptr;
}

FString SHuePanel::BuildSelectionKey(
    const TArray<UEdGraphNode*>& SupportedNodes,
    int32 TotalNodeCount) const
{
    TArray<FString> Parts;
    Parts.Reserve(SupportedNodes.Num());

    for (const UEdGraphNode* Node : SupportedNodes)
    {
        if (!Node)
        {
            continue;
        }

        FHueBlueprintCategoryInfo CategoryInfo;
        const bool bHasCategory =
            FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo);

        const FString CategoryKey = bHasCategory
            ? FString::Printf(
                TEXT("%s:%s"),
                CategoryInfo.DefiningBlueprint
                    ? *CategoryInfo.DefiningBlueprint->GetPathName()
                    : TEXT(""),
                *CategoryInfo.Category)
            : FString();

        Parts.Add(FString::Printf(
            TEXT("%u|%s|%s"),
            Node->GetUniqueID(),
            *FHueStyleResolver::GetGlobalKey(Node),
            *CategoryKey));
    }

    Parts.Sort();

    return FString::Printf(
        TEXT("%d:%d:%s"),
        TotalNodeCount,
        SupportedNodes.Num(),
        *FString::Join(Parts, TEXT(";")));
}

void SHuePanel::RefreshSelectionCache()
{
    TArray<UEdGraphNode*> SupportedNodes;
    GatherSelection(SupportedNodes, TotalSelectedNodeCount);

    SelectedNodes.Reset();
    for (UEdGraphNode* Node : SupportedNodes)
    {
        SelectedNodes.Add(Node);
    }

    CachedSelectionKey =
        BuildSelectionKey(SupportedNodes, TotalSelectedNodeCount);
}

void SHuePanel::RebuildPanel()
{
    if (!RootBox.IsValid())
    {
        return;
    }

    RootBox->ClearChildren();

    const TArray<UEdGraphNode*> Nodes = GetLiveSelectedNodes();
    const int32 SupportedCount = Nodes.Num();
    const int32 UnsupportedCount =
        FMath::Max(0, TotalSelectedNodeCount - SupportedCount);

    if (SupportedCount == 0)
    {
        RootBox->AddSlot()
        .AutoHeight()
        .Padding(10.0f)
        [
            SNew(STextBlock)
            .Text(TotalSelectedNodeCount > 0
                ? FText::Format(
                    LOCTEXT(
                        "NoSupportedSelection",
                        "{0} selected Blueprint nodes, but none are currently Hue-compatible."),
                    FText::AsNumber(TotalSelectedNodeCount))
                : LOCTEXT(
                    "SelectSupportedNode",
                    "Select one or more supported Blueprint nodes to edit their Hue style."))
            .ToolTipText(LOCTEXT(
                "SelectSupportedNodeTooltip",
                "Hue edits only nodes with a confirmed compatible visual path. Unsupported nodes can remain selected during a batch edit; Hue leaves them untouched."))
            .AutoWrapText(true)
            .ColorAndOpacity(FSlateColor::UseSubduedForeground())
        ];
        return;
    }

    RootBox->AddSlot()
    .AutoHeight()
    .Padding(10.0f, 8.0f, 10.0f, UnsupportedCount > 0 ? 2.0f : 8.0f)
    [
        SNew(STextBlock)
        .Text(SupportedCount == 1 && TotalSelectedNodeCount == 1
            ? Nodes[0]->GetNodeTitle(ENodeTitleType::ListView)
            : FText::Format(
                LOCTEXT(
                    "MultiSelectionTitle",
                    "{0} Hue-compatible nodes selected"),
                FText::AsNumber(SupportedCount)))
        .Font(FAppStyle::GetFontStyle("BoldFont"))
        .ToolTipText(SupportedCount == 1 && TotalSelectedNodeCount == 1
            ? LOCTEXT(
                "SelectedNodeTooltip",
                "Hue is editing visual style overrides for this selected Blueprint node.")
            : LOCTEXT(
                "SelectedNodesTooltip",
                "Hue edits the compatible portion of the current Blueprint graph selection as one batch operation."))
    ];

    if (UnsupportedCount > 0)
    {
        RootBox->AddSlot()
        .AutoHeight()
        .Padding(10.0f, 0.0f, 10.0f, 6.0f)
        [
            SNew(STextBlock)
            .Text(FText::Format(
                LOCTEXT(
                    "UnsupportedSelectedCount",
                    "{0} unsupported selected nodes will be left unchanged."),
                FText::AsNumber(UnsupportedCount)))
            .ToolTipText(LOCTEXT(
                "UnsupportedSelectedCountTooltip",
                "Hue never batch-edits nodes whose visual presentation has not been confirmed compatible."))
            .ColorAndOpacity(FSlateColor::UseSubduedForeground())
        ];
    }

    RootBox->AddSlot()
    .AutoHeight()
    .Padding(10.0f, 0.0f, 10.0f, 6.0f)
    [
        SNew(STextBlock)
        .Text(LOCTEXT(
            "PrecedenceSummary",
            "Instance  >  Category  >  Global  >  Unreal Default"))
        .ToolTipText(LOCTEXT(
            "PrecedenceSummaryTooltip",
            "Hue resolves each channel independently. Instance wins first; Category checks the exact Category and then nearest styled parent Categories; Global is next; otherwise Unreal's native value is used."))
        .ColorAndOpacity(FSlateColor::UseSubduedForeground())
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
    const TArray<UEdGraphNode*> Nodes = GetLiveSelectedNodes();
    UEdGraphNode* Node = GetPrimaryNode();

    const int32 ScopeTargetCount = GetScopeTargetCount(Scope);
    const int32 EligibleNodeCount = GetEligibleNodeCount(Scope);
    const bool bScopeAvailable = ScopeTargetCount > 0;
    const bool bMultiSelection = Nodes.Num() > 1 || TotalSelectedNodeCount > 1;

    FHueBlueprintCategoryInfo CategoryInfo;
    const bool bSingleCategoryAvailable =
        Nodes.Num() == 1
        && FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo);

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
                    "ScopeUnavailableTooltip",
                    "None of the selected Hue-compatible nodes have an applicable target for this scope."))
                .Visibility(!bScopeAvailable
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

    if (bMultiSelection)
    {
        Content->AddSlot()
        .AutoHeight()
        .Padding(6.0f, 5.0f, 6.0f, 1.0f)
        [
            SNew(STextBlock)
            .Text(Scope == EHueStyleScope::Instance
                ? FText::Format(
                    LOCTEXT(
                        "BatchInstanceSummary",
                        "Applies to {0} selected Hue-compatible nodes."),
                    FText::AsNumber(Nodes.Num()))
                : Scope == EHueStyleScope::Category
                    ? FText::Format(
                        LOCTEXT(
                            "BatchCategorySummary",
                            "Applies to {0} unique categories across {1} eligible selected nodes."),
                        FText::AsNumber(ScopeTargetCount),
                        FText::AsNumber(EligibleNodeCount))
                    : FText::Format(
                        LOCTEXT(
                            "BatchGlobalSummary",
                            "Applies to {0} unique global targets across {1} eligible selected nodes."),
                        FText::AsNumber(ScopeTargetCount),
                        FText::AsNumber(EligibleNodeCount)))
            .ToolTipText(LOCTEXT(
                "BatchScopeSummaryTooltip",
                "Hue deduplicates shared Category and Global identities before writing a batch edit, so the same shared rule is changed only once."))
            .AutoWrapText(true)
        ];

        const int32 IneligibleCount = Nodes.Num() - EligibleNodeCount;
        if (IneligibleCount > 0)
        {
            Content->AddSlot()
            .AutoHeight()
            .Padding(6.0f, 0.0f, 6.0f, 5.0f)
            [
                SNew(STextBlock)
                .Text(FText::Format(
                    LOCTEXT(
                        "BatchIneligibleSummary",
                        "{0} selected nodes have no target for this scope and will be ignored."),
                    FText::AsNumber(IneligibleCount)))
                .ColorAndOpacity(FSlateColor::UseSubduedForeground())
                .AutoWrapText(true)
            ];
        }
    }
    else if (Scope == EHueStyleScope::Category)
    {
        if (bSingleCategoryAvailable)
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
                    "This is the user-assigned My Blueprint Category on the member definition. Supported functions, macros, Blueprint-authored events, and Blueprint member variables in the same Category share these exact overrides. Nested child Categories inherit unset channels from their nearest styled parent."))
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
                .ColorAndOpacity(FSlateColor::UseSubduedForeground())
            ];
        }
        else
        {
            Content->AddSlot()
            .AutoHeight()
            .Padding(6.0f, 5.0f, 6.0f, 5.0f)
            [
                SNew(STextBlock)
                .Text(LOCTEXT(
                    "NoCategoryAssigned",
                    "No applicable My Blueprint Category is assigned to this node."))
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
            BuildChannelRow(Scope, Channels[ChannelIndex], bScopeAvailable)
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
        .Text(LOCTEXT("ClearScope", "Clear All Overrides"))
        .ToolTipText(FText::Format(
            LOCTEXT(
                "ClearScopeTooltip",
                "Clear every {0} Hue override represented by the current selection."),
            GetScopeLabel(Scope)))
        .IsEnabled_Lambda([this, Scope]()
        {
            return GetScopeTargetCount(Scope) > 0
                && HasAnyOverrideInSelection(Scope);
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
        .Padding(6.0f, 4.0f)
        [
            SNew(STextBlock)
            .Text(GetChannelLabel(Channel))
            .ToolTipText(ChannelTooltip)
        ]
        + SHorizontalBox::Slot()
        .AutoWidth()
        .VAlign(VAlign_Center)
        .Padding(8.0f, 4.0f, 4.0f, 4.0f)
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
                LOCTEXT(
                    "SetChannelTooltip",
                    "Choose {0} for the {1} scope. Batch edits replace only this Hue channel and leave the other channels unchanged. When values are mixed, the swatch and picker start from the first applicable target. {2}"),
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
                .Size(FVector2D(42.0f, 18.0f))
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
                LOCTEXT(
                    "ClearChannelTooltip",
                    "Clear this {0} override from every applicable target represented by the current selection."),
                GetChannelLabel(Channel)))
            .IsEnabled_Lambda([this, Scope, Channel]()
            {
                return HasColorOverrideInSelection(Scope, Channel);
            })
            .OnClicked(this, &SHuePanel::OnClearColorClicked, Scope, Channel)
        ];
}

FText SHuePanel::GetScopeLabel(EHueStyleScope Scope) const
{
    const TArray<UEdGraphNode*> Nodes = GetLiveSelectedNodes();

    if (Nodes.Num() <= 1 && TotalSelectedNodeCount <= 1)
    {
        switch (Scope)
        {
        case EHueStyleScope::Instance:
            return LOCTEXT("InstanceScope", "Instance");
        case EHueStyleScope::Category:
            return LOCTEXT("CategoryScope", "Category");
        case EHueStyleScope::GlobalFunction:
            return FHueStyleResolver::GetGlobalScopeLabel(GetPrimaryNode());
        }
    }

    const int32 Count = GetScopeTargetCount(Scope);

    switch (Scope)
    {
    case EHueStyleScope::Instance:
        return FText::Format(
            LOCTEXT("InstanceScopeBatch", "Instance ({0} Nodes)"),
            FText::AsNumber(Count));
    case EHueStyleScope::Category:
        return FText::Format(
            LOCTEXT("CategoryScopeBatch", "Category ({0} Categories)"),
            FText::AsNumber(Count));
    case EHueStyleScope::GlobalFunction:
        return FText::Format(
            LOCTEXT("GlobalScopeBatch", "Global ({0} Targets)"),
            FText::AsNumber(Count));
    }

    return FText::GetEmpty();
}

FText SHuePanel::GetScopeTooltip(EHueStyleScope Scope) const
{
    const TArray<UEdGraphNode*> Nodes = GetLiveSelectedNodes();

    if (Nodes.Num() > 1 || TotalSelectedNodeCount > 1)
    {
        const int32 TargetCount = GetScopeTargetCount(Scope);

        switch (Scope)
        {
        case EHueStyleScope::Instance:
            return FText::Format(
                LOCTEXT(
                    "InstanceScopeBatchTooltip",
                    "Edit the Instance overrides of {0} selected Hue-compatible nodes as one transaction. Click to collapse or expand."),
                FText::AsNumber(TargetCount));
        case EHueStyleScope::Category:
            return FText::Format(
                LOCTEXT(
                    "CategoryScopeBatchTooltip",
                    "Edit {0} unique Blueprint Category targets represented by the current selection. Nodes without an applicable Category are ignored. Click to collapse or expand."),
                FText::AsNumber(TargetCount));
        case EHueStyleScope::GlobalFunction:
            return FText::Format(
                LOCTEXT(
                    "GlobalScopeBatchTooltip",
                    "Edit {0} unique project-wide Hue identities represented by the current selection. Repeated calls to the same function are deduplicated. Click to collapse or expand."),
                FText::AsNumber(TargetCount));
        }
    }

    UEdGraphNode* Node = GetPrimaryNode();

    switch (Scope)
    {
    case EHueStyleScope::Instance:
        return LOCTEXT(
            "InstanceScopeTooltip",
            "Style only this selected node instance. Instance overrides have the highest Hue precedence. Click to collapse or expand this section.");

    case EHueStyleScope::Category:
    {
        FHueBlueprintCategoryInfo CategoryInfo;
        if (FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo))
        {
            return FText::Format(
                LOCTEXT(
                    "CategoryScopeTooltipAvailable",
                    "Shared style for supported Blueprint members in the My Blueprint Category '{0}' defined by {1}. Unset channels inherit from the nearest styled parent Category before Global or Unreal Default. Click to collapse or expand this section."),
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
            FHueStyleResolver::GetGlobalScopeTooltip(Node));
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
    const TArray<UEdGraphNode*> Nodes = GetLiveSelectedNodes();

    if (Nodes.Num() == 1 && TotalSelectedNodeCount <= 1)
    {
        UEdGraphNode* Node = Nodes[0];
        if (!HuePanelPrivate::IsScopeApplicable(Node, Scope))
        {
            return LOCTEXT("UnavailableState", "Unavailable");
        }

        if (FHueStyleResolver::HasColorOverride(Node, Scope, Channel))
        {
            return LOCTEXT("OverrideState", "Override");
        }

        return GetSingleSelectionSourceText(Scope, Channel);
    }

    bool bFoundApplicable = false;
    bool bFirstHasOverride = false;
    bool bMixed = false;
    FLinearColor FirstColor = FLinearColor::White;

    for (UEdGraphNode* Node : Nodes)
    {
        if (!HuePanelPrivate::IsScopeApplicable(Node, Scope))
        {
            continue;
        }

        FLinearColor Color;
        const bool bHasOverride =
            FHueStyleResolver::GetColorOverride(
                Node,
                Scope,
                Channel,
                Color);

        const FLinearColor DisplayedColor = bHasOverride
            ? Color
            : FHueStyleResolver::GetPickerInitialColor(
                Node,
                Scope,
                Channel);

        if (!bFoundApplicable)
        {
            bFoundApplicable = true;
            bFirstHasOverride = bHasOverride;
            FirstColor = DisplayedColor;
            continue;
        }

        if (bHasOverride != bFirstHasOverride
            || !HuePanelPrivate::NearlyEqualColor(DisplayedColor, FirstColor))
        {
            bMixed = true;
            break;
        }
    }

    if (!bFoundApplicable)
    {
        return LOCTEXT("UnavailableState", "Unavailable");
    }

    if (bMixed)
    {
        return LOCTEXT("MultipleValuesState", "Multiple Values");
    }

    return bFirstHasOverride
        ? LOCTEXT("OverrideState", "Override")
        : LOCTEXT("InheritedState", "Inherited");
}

FText SHuePanel::GetSingleSelectionSourceText(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    UEdGraphNode* Node = GetPrimaryNode();
    if (!Node)
    {
        return FText::GetEmpty();
    }

    if (Scope == EHueStyleScope::Instance)
    {
        FString CategorySource;
        FLinearColor CategoryColor;
        if (FHueStyleResolver::GetEffectiveCategoryColor(
            Node,
            Channel,
            CategoryColor,
            &CategorySource))
        {
            return FText::Format(
                LOCTEXT("SourceCategoryShort", "Category: {0}"),
                FText::FromString(CategorySource));
        }

        if (FHueStyleResolver::HasColorOverride(
            Node,
            EHueStyleScope::GlobalFunction,
            Channel))
        {
            return FHueStyleResolver::GetGlobalScopeLabel(Node);
        }

        return LOCTEXT("SourceUnrealDefault", "Unreal Default");
    }

    if (Scope == EHueStyleScope::Category)
    {
        FString ParentCategorySource;
        FLinearColor ParentCategoryColor;
        if (FHueStyleResolver::GetEffectiveCategoryColor(
            Node,
            Channel,
            ParentCategoryColor,
            &ParentCategorySource,
            false))
        {
            return FText::Format(
                LOCTEXT("SourceParentCategoryShort", "Parent: {0}"),
                FText::FromString(ParentCategorySource));
        }

        if (FHueStyleResolver::HasColorOverride(
            Node,
            EHueStyleScope::GlobalFunction,
            Channel))
        {
            return FHueStyleResolver::GetGlobalScopeLabel(Node);
        }

        return LOCTEXT("SourceUnrealDefault", "Unreal Default");
    }

    return LOCTEXT("SourceUnrealDefault", "Unreal Default");
}

FText SHuePanel::GetInheritedSourceTooltip(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    UEdGraphNode* Node = GetPrimaryNode();
    if (!Node)
    {
        return FText::GetEmpty();
    }

    if (Scope == EHueStyleScope::Instance)
    {
        FHueBlueprintCategoryInfo CategoryInfo;
        FString CategorySource;
        FLinearColor CategoryColor;
        if (FHueStyleResolver::GetBlueprintCategoryInfo(Node, CategoryInfo)
            && FHueStyleResolver::GetEffectiveCategoryColor(
                Node,
                Channel,
                CategoryColor,
                &CategorySource))
        {
            return FText::Format(
                LOCTEXT(
                    "InheritedFromCategoryFmt",
                    "Inherited from Category '{0}' defined by {1}. Hue resolves the nearest active Category value below Instance for this channel."),
                FText::FromString(CategorySource),
                FText::FromString(
                    CategoryInfo.DefiningBlueprint
                        ? CategoryInfo.DefiningBlueprint->GetName()
                        : FString()));
        }

        if (FHueStyleResolver::HasColorOverride(Node, EHueStyleScope::GlobalFunction, Channel))
        {
            return FText::Format(
                LOCTEXT(
                    "InheritedFromGlobalFmt",
                    "Inherited from {0}. No higher-precedence Category value is active for this channel."),
                FHueStyleResolver::GetGlobalScopeLabel(Node));
        }

        return LOCTEXT(
            "InheritedFromNativeInstance",
            "Inherited from Unreal Default. Neither Category nor the applicable Global scope overrides this channel.");
    }

    if (Scope == EHueStyleScope::Category)
    {
        FString ParentCategorySource;
        FLinearColor ParentCategoryColor;
        if (FHueStyleResolver::GetEffectiveCategoryColor(
            Node,
            Channel,
            ParentCategoryColor,
            &ParentCategorySource,
            false))
        {
            return FText::Format(
                LOCTEXT(
                    "CategoryInheritedFromParentFmt",
                    "Inherited from parent Category '{0}'. The nearest parent with an override wins for this channel."),
                FText::FromString(ParentCategorySource));
        }

        if (FHueStyleResolver::HasColorOverride(Node, EHueStyleScope::GlobalFunction, Channel))
        {
            return FText::Format(
                LOCTEXT(
                    "CategoryInheritedFromGlobalFmt",
                    "Inherited from {0}. Neither this Category nor any parent Category overrides this channel."),
                FHueStyleResolver::GetGlobalScopeLabel(Node));
        }

        return LOCTEXT(
            "CategoryInheritedFromNative",
            "Inherited from Unreal Default. Neither this Category, a parent Category, nor the applicable Global scope overrides this channel.");
    }

    return LOCTEXT(
        "GlobalInheritedFromNative",
        "Inherited from Unreal Default. No project-wide Hue override is set for this channel.");
}

FText SHuePanel::GetOverrideStateTooltip(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    const TArray<UEdGraphNode*> Nodes = GetLiveSelectedNodes();
    const FText State = GetOverrideStateText(Scope, Channel);

    if (State.EqualTo(LOCTEXT("MultipleValuesState", "Multiple Values")))
    {
        return FText::Format(
            LOCTEXT(
                "MultipleValuesTooltip",
                "The selected targets differ in explicit override state or in the value currently displayed through inheritance. Choosing a color will replace this channel across {0} unique scope targets."),
            FText::AsNumber(GetScopeTargetCount(Scope)));
    }

    if (Nodes.Num() > 1 || TotalSelectedNodeCount > 1)
    {
        if (State.EqualTo(LOCTEXT("UnavailableState", "Unavailable")))
        {
            return LOCTEXT(
                "UnavailableBatchTooltip",
                "None of the selected Hue-compatible nodes have an applicable target for this scope.");
        }

        return State.EqualTo(LOCTEXT("OverrideState", "Override"))
            ? LOCTEXT(
                "SharedOverrideBatchTooltip",
                "All applicable selected targets share the same explicit Hue value for this channel.")
            : LOCTEXT(
                "SharedInheritedBatchTooltip",
                "All applicable selected targets currently inherit this channel rather than defining an explicit override at this scope.");
    }

    UEdGraphNode* Node = GetPrimaryNode();
    if (!Node)
    {
        return FText::GetEmpty();
    }

    if (State.EqualTo(LOCTEXT("UnavailableState", "Unavailable")))
    {
        return LOCTEXT(
            "UnavailableStateTooltip",
            "No applicable target exists for this Hue scope.");
    }

    if (FHueStyleResolver::HasColorOverride(Node, Scope, Channel))
    {
        return FText::Format(
            LOCTEXT(
                "OverrideStateTooltip",
                "The {0} scope explicitly owns this channel. Clearing it reveals the next available lower-precedence value."),
            GetScopeLabel(Scope));
    }

    return GetInheritedSourceTooltip(Scope, Channel);
}

FLinearColor SHuePanel::GetSwatchColor(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    const TArray<UEdGraphNode*> Nodes = GetLiveSelectedNodes();

    for (UEdGraphNode* Node : Nodes)
    {
        if (HuePanelPrivate::IsScopeApplicable(Node, Scope))
        {
            return FHueStyleResolver::GetPickerInitialColor(
                Node,
                Scope,
                Channel);
        }
    }

    return FLinearColor::White;
}

int32 SHuePanel::GetScopeTargetCount(EHueStyleScope Scope) const
{
    return FHueStyleResolver::GetUniqueScopeTargetCount(
        GetLiveSelectedNodes(),
        Scope);
}

int32 SHuePanel::GetEligibleNodeCount(EHueStyleScope Scope) const
{
    int32 Count = 0;

    for (UEdGraphNode* Node : GetLiveSelectedNodes())
    {
        if (HuePanelPrivate::IsScopeApplicable(Node, Scope))
        {
            ++Count;
        }
    }

    return Count;
}

bool SHuePanel::HasAnyOverrideInSelection(EHueStyleScope Scope) const
{
    for (UEdGraphNode* Node : GetLiveSelectedNodes())
    {
        if (HuePanelPrivate::IsScopeApplicable(Node, Scope)
            && FHueStyleResolver::HasAnyOverride(Node, Scope))
        {
            return true;
        }
    }

    return false;
}

bool SHuePanel::HasColorOverrideInSelection(
    EHueStyleScope Scope,
    EHueStyleChannel Channel) const
{
    for (UEdGraphNode* Node : GetLiveSelectedNodes())
    {
        if (HuePanelPrivate::IsScopeApplicable(Node, Scope)
            && FHueStyleResolver::HasColorOverride(Node, Scope, Channel))
        {
            return true;
        }
    }

    return false;
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

const FSlateBrush* SHuePanel::GetScopeExpansionBrush(
    EHueStyleScope Scope) const
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
    const TArray<UEdGraphNode*> Nodes = GetLiveSelectedNodes();
    if (FHueStyleResolver::GetUniqueScopeTargetCount(Nodes, Scope) == 0)
    {
        return FReply::Handled();
    }

    TArray<TWeakObjectPtr<UEdGraphNode>> WeakNodes;
    WeakNodes.Reserve(Nodes.Num());

    UEdGraphNode* InitialNode = nullptr;
    for (UEdGraphNode* Node : Nodes)
    {
        WeakNodes.Add(Node);
        if (!InitialNode && HuePanelPrivate::IsScopeApplicable(Node, Scope))
        {
            InitialNode = Node;
        }
    }

    if (!InitialNode)
    {
        return FReply::Handled();
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
                TArray<UEdGraphNode*> LiveNodes;
                LiveNodes.Reserve(WeakNodes.Num());

                for (const TWeakObjectPtr<UEdGraphNode>& WeakNode : WeakNodes)
                {
                    if (UEdGraphNode* Node = WeakNode.Get())
                    {
                        LiveNodes.Add(Node);
                    }
                }

                FHueStyleResolver::SetColorOverrides(
                    LiveNodes,
                    Scope,
                    Channel,
                    NewColor);
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
    FHueStyleResolver::ClearColorOverrides(
        GetLiveSelectedNodes(),
        Scope,
        Channel);

    return FReply::Handled();
}

FReply SHuePanel::OnClearScopeClicked(EHueStyleScope Scope)
{
    FHueStyleResolver::ClearAllOverrides(
        GetLiveSelectedNodes(),
        Scope);

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
