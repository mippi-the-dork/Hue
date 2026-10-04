// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeK2Operator.h"

#include "EdGraph/EdGraph.h"
#include "K2Node_PromotableOperator.h"
#include "Widgets/SBoxPanel.h"

#define LOCTEXT_NAMESPACE "HueGraphNodeK2Operator"

void SHueGraphNodeK2Operator::Construct(
    const FArguments& InArgs,
    UK2Node_PromotableOperator* InNode)
{
    SHueGraphNodeCallFunction::Construct(
        SHueGraphNodeCallFunction::FArguments(),
        InNode);
}

UK2Node_PromotableOperator* SHueGraphNodeK2Operator::GetOperatorNode() const
{
    return Cast<UK2Node_PromotableOperator>(GraphNode);
}

void SHueGraphNodeK2Operator::CreateInputSideAddButton(
    TSharedPtr<SVerticalBox> InputBox)
{
    UK2Node_PromotableOperator* OperatorNode = GetOperatorNode();
    if (!InputBox.IsValid() || !OperatorNode || !OperatorNode->CanAddPin())
    {
        return;
    }

    InputBox->AddSlot()
    .AutoHeight()
    .VAlign(VAlign_Center)
    .Padding(0.0f, 4.0f, 0.0f, 0.0f)
    [
        AddPinButtonContent(
            LOCTEXT("AddOperand", "Add pin"),
            LOCTEXT("AddOperandTooltip", "Add another operand input to this operator."),
            false)
    ];
}

FReply SHueGraphNodeK2Operator::OnAddPin()
{
    UK2Node_PromotableOperator* OperatorNode = GetOperatorNode();
    if (!OperatorNode || !OperatorNode->CanAddPin())
    {
        return FReply::Handled();
    }

    OperatorNode->Modify();
    OperatorNode->AddInputPin();

    if (UEdGraph* Graph = OperatorNode->GetGraph())
    {
        Graph->NotifyGraphChanged();
    }

    UpdateGraphNode();
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
