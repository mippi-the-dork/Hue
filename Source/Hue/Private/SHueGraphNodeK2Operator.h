// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "SHueGraphNodeCallFunction.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SVerticalBox;
class UK2Node_PromotableOperator;

/**
 * Hue presentation for UE5 promotable math operators such as Add, Subtract,
 * Multiply, Divide, and comparison operators.
 *
 * The operator keeps Hue's shared compact presentation support and recreates
 * only the public Add Operand interaction that the specialized native widget
 * contributes beyond the default K2 presentation.
 */
class SHueGraphNodeK2Operator : public SHueGraphNodeCallFunction
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeK2Operator) {}
    SLATE_END_ARGS()

    void Construct(
        const FArguments& InArgs,
        UK2Node_PromotableOperator* InNode);

protected:
    virtual void CreateInputSideAddButton(
        TSharedPtr<SVerticalBox> InputBox) override;

    virtual FReply OnAddPin() override;

private:
    UK2Node_PromotableOperator* GetOperatorNode() const;
};
