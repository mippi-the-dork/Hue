// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueTabSummoner.h"

#include "BlueprintEditor.h"
#include "SHuePanel.h"

#define LOCTEXT_NAMESPACE "HueTabSummoner"

const FName FHueTabSummoner::TabID(TEXT("Hue"));

FHueTabSummoner::FHueTabSummoner(TSharedPtr<FBlueprintEditor> InBlueprintEditor)
    : FWorkflowTabFactory(TabID, InBlueprintEditor)
    , BlueprintEditorPtr(InBlueprintEditor)
{
    TabLabel = LOCTEXT("HueTabLabel", "Hue");
    ViewMenuDescription = LOCTEXT("HueViewMenuDescription", "Hue");
    ViewMenuTooltip = LOCTEXT("HueViewMenuTooltip", "Open the Hue function-node styling panel.");
    bIsSingleton = true;
}

TSharedRef<SWidget> FHueTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
    return SNew(SHuePanel)
        .BlueprintEditor(BlueprintEditorPtr);
}

FText FHueTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
    return LOCTEXT(
        "HueTabTooltip",
        "Edit Hue style overrides for the selected supported Blueprint function-call node.");
}

#undef LOCTEXT_NAMESPACE
