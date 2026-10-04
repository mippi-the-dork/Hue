// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "SHueGraphNodeCallFunction.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UK2Node_Timeline;
struct FGraphInformationPopupInfo;
struct FNodeInfoContext;

/**
 * Hue-compatible Timeline presentation.
 *
 * Timeline uses the normal K2 node layout plus runtime debug information. Hue
 * keeps the default visual behavior and restores the Timeline state popup that
 * Unreal's specialized Timeline Slate widget contributes.
 */
class SHueGraphNodeK2Timeline : public SHueGraphNodeCallFunction
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeK2Timeline) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UK2Node_Timeline* InNode);

    virtual void GetNodeInfoPopups(
        FNodeInfoContext* Context,
        TArray<FGraphInformationPopupInfo>& Popups) const override;
};
