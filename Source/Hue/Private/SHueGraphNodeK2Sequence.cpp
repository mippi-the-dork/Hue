// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeK2Sequence.h"

#include "EdGraph/EdGraph.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "ScopedTransaction.h"
#include "Widgets/SBoxPanel.h"

#define LOCTEXT_NAMESPACE "HueGraphNodeK2Sequence"

void SHueGraphNodeK2Sequence::Construct(const FArguments& InArgs, UK2Node* InNode)
{
    SHueGraphNodeCallFunction::Construct(
        SHueGraphNodeCallFunction::FArguments(),
        InNode);
}

void SHueGraphNodeK2Sequence::CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox)
{
    if (!OutputBox.IsValid())
    {
        return;
    }

    UK2Node* Node = Cast<UK2Node>(GraphNode);
    IK2Node_AddPinInterface* AddPinNode = Node
        ? Cast<IK2Node_AddPinInterface>(Node)
        : nullptr;
    if (!AddPinNode)
    {
        return;
    }

    OutputBox->AddSlot()
    .AutoHeight()
    .VAlign(VAlign_Center)
    .Padding(0.0f, 4.0f, 0.0f, 0.0f)
    [
        AddPinButtonContent(
            LOCTEXT("AddPin", "Add pin"),
            LOCTEXT("AddPinTooltip", "Add another pin to this node."),
            true)
    ];
}

FReply SHueGraphNodeK2Sequence::OnAddPin()
{
    UK2Node* Node = Cast<UK2Node>(GraphNode);
    IK2Node_AddPinInterface* AddPinNode = Node
        ? Cast<IK2Node_AddPinInterface>(Node)
        : nullptr;
    if (!AddPinNode || !AddPinNode->CanAddPin())
    {
        return FReply::Handled();
    }

    // Match Unreal's native SGraphNodeK2Sequence behavior. Add-pin nodes must
    // participate in the editor transaction stack so Select, Sequence,
    // MultiGate, Make Array/Set/Map, and other IK2Node_AddPinInterface nodes
    // keep native Undo/Redo semantics while Hue owns their Slate wrapper.
    const FScopedTransaction Transaction(
        LOCTEXT("AddPinTransaction", "Add Pin"));

    Node->Modify();
    AddPinNode->AddInputPin();

    UpdateGraphNode();

    if (UEdGraph* Graph = Node->GetGraph())
    {
        Graph->NotifyGraphChanged();
    }

    return FReply::Handled();
}

EVisibility SHueGraphNodeK2Sequence::IsAddPinButtonVisible() const
{
    UK2Node* Node = Cast<UK2Node>(GraphNode);
    IK2Node_AddPinInterface* AddPinNode = Node
        ? Cast<IK2Node_AddPinInterface>(Node)
        : nullptr;

    return (AddPinNode && AddPinNode->CanAddPin())
        ? EVisibility::Visible
        : EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE
