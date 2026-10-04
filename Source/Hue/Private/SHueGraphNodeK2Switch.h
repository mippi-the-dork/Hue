// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "SHueGraphNodeCallFunction.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SVerticalBox;
class UK2Node_Switch;

/**
 * Hue presentation for UK2Node_Switch.
 *
 * Unreal normally uses a specialized switch Slate widget. Hue keeps the
 * standard K2 presentation and restores the public switch Add Case behavior
 * directly through UK2Node_Switch so case management stays on the native node.
 */
class SHueGraphNodeK2Switch : public SHueGraphNodeCallFunction
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeK2Switch) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UK2Node_Switch* InNode);

protected:
    virtual void CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox) override;
    virtual FReply OnAddPin() override;

private:
    UK2Node_Switch* GetSwitchNode() const;
};
