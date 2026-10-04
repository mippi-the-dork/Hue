// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeK2Switch.h"

#include "EdGraph/EdGraph.h"
#include "K2Node_Switch.h"
#include "Widgets/SBoxPanel.h"

#define LOCTEXT_NAMESPACE "HueGraphNodeK2Switch"

void SHueGraphNodeK2Switch::Construct(
    const FArguments& InArgs,
    UK2Node_Switch* InNode)
{
    SHueGraphNodeCallFunction::Construct(
        SHueGraphNodeCallFunction::FArguments(),
        InNode);
}

UK2Node_Switch* SHueGraphNodeK2Switch::GetSwitchNode() const
{
    return Cast<UK2Node_Switch>(GraphNode);
}

void SHueGraphNodeK2Switch::CreateOutputSideAddButton(
    TSharedPtr<SVerticalBox> OutputBox)
{
    UK2Node_Switch* SwitchNode = GetSwitchNode();
    if (!OutputBox.IsValid()
        || !SwitchNode
        || !SwitchNode->SupportsAddPinButton())
    {
        return;
    }

    OutputBox->AddSlot()
    .AutoHeight()
    .VAlign(VAlign_Center)
    .Padding(0.0f, 4.0f, 0.0f, 0.0f)
    [
        AddPinButtonContent(
            LOCTEXT("AddCase", "Add pin"),
            LOCTEXT("AddCaseTooltip", "Add another case pin to this switch."),
            true)
    ];
}

FReply SHueGraphNodeK2Switch::OnAddPin()
{
    UK2Node_Switch* SwitchNode = GetSwitchNode();
    if (!SwitchNode || !SwitchNode->SupportsAddPinButton())
    {
        return FReply::Handled();
    }

    SwitchNode->Modify();
    SwitchNode->AddPinToSwitchNode();

    if (UEdGraph* Graph = SwitchNode->GetGraph())
    {
        Graph->NotifyGraphChanged();
    }

    UpdateGraphNode();
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
