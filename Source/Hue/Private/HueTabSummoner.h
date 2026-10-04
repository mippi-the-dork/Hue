// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FBlueprintEditor;

class FHueTabSummoner : public FWorkflowTabFactory
{
public:
    static const FName TabID;

    explicit FHueTabSummoner(TSharedPtr<FBlueprintEditor> InBlueprintEditor);

    virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
    virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
    TWeakPtr<FBlueprintEditor> BlueprintEditorPtr;
};
