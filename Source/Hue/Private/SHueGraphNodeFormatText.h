// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "SHueGraphNodeCallFunction.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SVerticalBox;
class UK2Node_FormatText;

/**
 * Hue compatibility presentation for Format Text.
 *
 * The node UObject owns argument names, pin reconstruction, and compilation.
 * Hue restores the public Add Argument interaction while keeping native pin
 * widgets and default value editing.
 */
class SHueGraphNodeFormatText : public SHueGraphNodeCallFunction
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeFormatText) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UK2Node_FormatText* InNode);

protected:
    virtual void CreateInputSideAddButton(
        TSharedPtr<SVerticalBox> InputBox) override;

    virtual FReply OnAddPin() override;

private:
    UK2Node_FormatText* GetFormatTextNode() const;
};
