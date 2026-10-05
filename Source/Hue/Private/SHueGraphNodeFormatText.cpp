// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeFormatText.h"

#include "EdGraph/EdGraph.h"
#include "GraphEditorSettings.h"
#include "K2Node_FormatText.h"
#include "Widgets/SBoxPanel.h"

#define LOCTEXT_NAMESPACE "HueGraphNodeFormatText"

void SHueGraphNodeFormatText::Construct(
    const FArguments& InArgs,
    UK2Node_FormatText* InNode)
{
    SHueGraphNodeCallFunction::Construct(
        SHueGraphNodeCallFunction::FArguments(),
        InNode);
}

UK2Node_FormatText* SHueGraphNodeFormatText::GetFormatTextNode() const
{
    return Cast<UK2Node_FormatText>(GraphNode);
}

void SHueGraphNodeFormatText::CreateInputSideAddButton(
    TSharedPtr<SVerticalBox> InputBox)
{
    UK2Node_FormatText* FormatNode = GetFormatTextNode();
    if (!InputBox.IsValid()
        || !FormatNode
        || !FormatNode->CanEditArguments())
    {
        return;
    }

    InputBox->AddSlot()
    .AutoHeight()
    .HAlign(HAlign_Left)
    .VAlign(VAlign_Center)
    .Padding(Settings->GetInputPinPadding())
    [
        AddPinButtonContent(
            LOCTEXT("AddArgument", "Add pin"),
            LOCTEXT("AddArgumentTooltip", "Add another Format Text argument pin."),
            false)
    ];
}

FReply SHueGraphNodeFormatText::OnAddPin()
{
    UK2Node_FormatText* FormatNode = GetFormatTextNode();
    if (!FormatNode || !FormatNode->CanEditArguments())
    {
        return FReply::Handled();
    }

    FormatNode->Modify();
    FormatNode->AddArgumentPin();

    if (UEdGraph* Graph = FormatNode->GetGraph())
    {
        Graph->NotifyGraphChanged();
    }

    UpdateGraphNode();
    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
