// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphPinExec.h"

#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "HueStyleResolver.h"
#include "K2Node.h"

void SHueGraphPinExec::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
    SGraphPinExec::Construct(SGraphPinExec::FArguments(), InPin);
}

FSlateColor SHueGraphPinExec::GetPinColor() const
{
    UEdGraphPin* Pin = GetPinObj();
    if (!Pin || Pin->bOrphanedPin)
    {
        return SGraphPinExec::GetPinColor();
    }

    const UK2Node* Node = Cast<UK2Node>(Pin->GetOwningNodeUnchecked());
    if (!FHueStyleResolver::IsSupportedNode(Node))
    {
        return SGraphPinExec::GetPinColor();
    }

    FLinearColor BodyTextColor;
    if (!FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::BodyTextColor, BodyTextColor))
    {
        return SGraphPinExec::GetPinColor();
    }

    if (!Node->IsNodeEnabled()
        || Node->IsDisplayAsDisabledForced()
        || Node->IsNodeUnrelated()
        || !IsEditingEnabled())
    {
        BodyTextColor *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return FSlateColor(BodyTextColor);
}
