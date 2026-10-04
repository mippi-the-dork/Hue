// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "SHueGraphNodeCallFunction.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SVerticalBox;
class UK2Node;

/**
 * Hue rendering for nodes that implement IK2Node_AddPinInterface.
 * Unreal's SGraphNodeK2Sequence is not available to external plugins in the
 * installed Engine build tested with Hue, so Hue reproduces the public add-pin
 * behavior on top of Hue's default K2 presentation.
 */
class SHueGraphNodeK2Sequence : public SHueGraphNodeCallFunction
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeK2Sequence) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UK2Node* InNode);

protected:
    virtual void CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox) override;
    virtual FReply OnAddPin() override;
    virtual EVisibility IsAddPinButtonVisible() const override;
};
