// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "HueTypes.h"
#include "Widgets/SCompoundWidget.h"

class FBlueprintEditor;
class UBlueprint;
class UEdGraphNode;
class SVerticalBox;
struct FSlateBrush;

class SHuePanel : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SHuePanel) {}
        SLATE_ARGUMENT(TWeakPtr<FBlueprintEditor>, BlueprintEditor)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    virtual void Tick(
        const FGeometry& AllottedGeometry,
        const double InCurrentTime,
        const float InDeltaTime) override;

private:
    static FText GetChannelLabel(EHueStyleChannel Channel);
    static FText GetChannelTooltip(EHueStyleChannel Channel);

    UEdGraphNode* GetSelectedNode() const;
    void RebuildPanel();

    TSharedRef<SWidget> BuildScopeSection(EHueStyleScope Scope);
    TSharedRef<SWidget> BuildChannelRow(
        EHueStyleScope Scope,
        EHueStyleChannel Channel,
        bool bScopeEnabled = true);

    FText GetScopeLabel(EHueStyleScope Scope) const;
    FText GetScopeTooltip(EHueStyleScope Scope) const;
    FText GetOverrideStateText(EHueStyleScope Scope, EHueStyleChannel Channel) const;
    FText GetOverrideStateTooltip(EHueStyleScope Scope, EHueStyleChannel Channel) const;
    FText GetInheritedSourceTooltip(EHueStyleScope Scope, EHueStyleChannel Channel) const;
    FLinearColor GetSwatchColor(EHueStyleScope Scope, EHueStyleChannel Channel) const;

    bool IsScopeExpanded(EHueStyleScope Scope) const;
    const FSlateBrush* GetScopeExpansionBrush(EHueStyleScope Scope) const;

    FReply OnScopeHeaderClicked(EHueStyleScope Scope);
    FReply OnSetColorClicked(EHueStyleScope Scope, EHueStyleChannel Channel);
    FReply OnClearColorClicked(EHueStyleScope Scope, EHueStyleChannel Channel);
    FReply OnClearScopeClicked(EHueStyleScope Scope);
    FReply OnOpenHueSettingsClicked();

    TWeakPtr<FBlueprintEditor> BlueprintEditorPtr;
    TWeakObjectPtr<UEdGraphNode> SelectedNode;
    bool bCachedCategoryAvailable = false;
    FString CachedCategory;
    TWeakObjectPtr<UBlueprint> CachedCategoryBlueprint;

    bool bInstanceExpanded = true;
    bool bCategoryExpanded = true;
    bool bGlobalExpanded = true;

    TSharedPtr<SVerticalBox> RootBox;
};
