// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeK2Event.h"

#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "HueStyleResolver.h"
#include "K2Node_Event.h"
#include "SGraphPin.h"
#include "SHueGraphPinExec.h"

void SHueGraphNodeK2Event::Construct(
    const FArguments& InArgs,
    UK2Node_Event* InNode)
{
    SGraphNodeK2Default::Construct(SGraphNodeK2Default::FArguments(), InNode);
}

const UK2Node_Event* SHueGraphNodeK2Event::GetHueNode() const
{
    return Cast<UK2Node_Event>(GraphNode);
}

FSlateColor SHueGraphNodeK2Event::GetNodeTitleColor() const
{
    const UK2Node_Event* Node = GetHueNode();
    if (!Node || Node->IsDeprecated())
    {
        return SGraphNodeK2Event::GetNodeTitleColor();
    }

    FLinearColor Color;
    if (!FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::HeaderColor, Color))
    {
        return SGraphNodeK2Event::GetNodeTitleColor();
    }

    if (!Node->IsNodeEnabled() || Node->IsDisplayAsDisabledForced() || Node->IsNodeUnrelated())
    {
        Color *= FLinearColor(0.5f, 0.5f, 0.5f, 0.4f);
    }
    else
    {
        Color.A = FadeCurve.GetLerp();
    }

    return FSlateColor(Color);
}

FLinearColor SHueGraphNodeK2Event::GetNodeTitleTextColor() const
{
    const UK2Node_Event* Node = GetHueNode();

    FLinearColor Color;
    if (!Node || !FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::HeaderTextColor, Color))
    {
        return SGraphNodeK2Event::GetNodeTitleTextColor();
    }

    if (!Node->IsNodeEnabled() || Node->IsDisplayAsDisabledForced() || Node->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return Color;
}

FSlateColor SHueGraphNodeK2Event::GetNodeBodyColor() const
{
    const UK2Node_Event* Node = GetHueNode();

    FLinearColor Color;
    if (!Node || !FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::BodyColor, Color))
    {
        return SGraphNodeK2Event::GetNodeBodyColor();
    }

    if (!Node->IsNodeEnabled() || Node->IsDisplayAsDisabledForced() || Node->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return FSlateColor(Color);
}

TOptional<FSlateColor> SHueGraphNodeK2Event::GetPinTextColor(
    const SGraphPin* InGraphPin) const
{
    const UK2Node_Event* Node = GetHueNode();
    if (!Node || !InGraphPin)
    {
        return TOptional<FSlateColor>();
    }

    const UEdGraphPin* PinObject = InGraphPin->GetPinObj();
    if (PinObject && PinObject->bOrphanedPin)
    {
        return TOptional<FSlateColor>();
    }

    FLinearColor Color;
    if (!FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::BodyTextColor, Color))
    {
        return TOptional<FSlateColor>();
    }

    if (!Node->IsNodeEnabled()
        || Node->IsDisplayAsDisabledForced()
        || Node->IsNodeUnrelated()
        || !InGraphPin->IsEditingEnabled())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return FSlateColor(Color);
}

TSharedPtr<SGraphPin> SHueGraphNodeK2Event::CreatePinWidget(UEdGraphPin* Pin) const
{
    if (Pin && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
    {
        return SNew(SHueGraphPinExec, Pin);
    }

    return SGraphNodeK2Event::CreatePinWidget(Pin);
}

TSharedRef<SWidget> SHueGraphNodeK2Event::CreateTitleWidget(
    TSharedPtr<SNodeTitle> NodeTitle)
{
    TSharedRef<SWidget> MainTitle =
        SGraphNodeK2Event::CreateTitleWidget(NodeTitle);

    if (NodeTitle.IsValid())
    {
        NodeTitle->SetColorAndOpacity(
            TAttribute<FLinearColor>::Create(
                TAttribute<FLinearColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeK2Event::GetNodeTitleTextColor)));
    }

    return MainTitle;
}
