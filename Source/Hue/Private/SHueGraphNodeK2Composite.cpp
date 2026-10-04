// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeK2Composite.h"

#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "HueStyleResolver.h"
#include "K2Node_Composite.h"
#include "Layout/Children.h"
#include "SGraphPin.h"
#include "SHueGraphPinExec.h"
#include "Styling/AppStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

namespace HueGraphNodeK2CompositePrivate
{
    static SBorder* FindCompositeBodyBorder(SWidget& Widget)
    {
        if (Widget.GetType() == SBorder::StaticWidgetClass().GetWidgetType())
        {
            SBorder& Border = static_cast<SBorder&>(Widget);
            if (Border.GetBorderImage()
                == FAppStyle::GetBrush(TEXT("Graph.CollapsedNode.Body")))
            {
                return &Border;
            }
        }

        FChildren* Children = Widget.GetChildren();
        if (!Children)
        {
            return nullptr;
        }

        for (int32 ChildIndex = 0; ChildIndex < Children->Num(); ++ChildIndex)
        {
            TSharedRef<SWidget> Child = Children->GetChildAt(ChildIndex);
            if (SBorder* Match = FindCompositeBodyBorder(Child.Get()))
            {
                return Match;
            }
        }

        return nullptr;
    }

    static SNodeTitle* FindNodeTitle(SWidget& Widget)
    {
        if (Widget.GetType() == SNodeTitle::StaticWidgetClass().GetWidgetType())
        {
            return static_cast<SNodeTitle*>(&Widget);
        }

        FChildren* Children = Widget.GetChildren();
        if (!Children)
        {
            return nullptr;
        }

        for (int32 ChildIndex = 0; ChildIndex < Children->Num(); ++ChildIndex)
        {
            TSharedRef<SWidget> Child = Children->GetChildAt(ChildIndex);
            if (SNodeTitle* Match = FindNodeTitle(Child.Get()))
            {
                return Match;
            }
        }

        return nullptr;
    }
}

void SHueGraphNodeK2Composite::Construct(
    const FArguments& InArgs,
    UK2Node_Composite* InNode)
{
    SGraphNodeK2Composite::Construct(
        SGraphNodeK2Composite::FArguments(),
        InNode);
}

const UK2Node_Composite* SHueGraphNodeK2Composite::GetHueNode() const
{
    return Cast<UK2Node_Composite>(GraphNode);
}

void SHueGraphNodeK2Composite::UpdateGraphNode()
{
    SGraphNodeK2Composite::UpdateGraphNode();
    BindHueToNativeCompositeLayers();
}

FSlateColor SHueGraphNodeK2Composite::GetNodeTitleColor() const
{
    const UK2Node_Composite* Node = GetHueNode();
    if (!Node || Node->IsDeprecated())
    {
        return SGraphNodeK2Composite::GetNodeTitleColor();
    }

    FLinearColor Color;
    if (!FHueStyleResolver::ResolveColor(
        Node,
        EHueStyleChannel::HeaderColor,
        Color))
    {
        return SGraphNodeK2Composite::GetNodeTitleColor();
    }

    if (!Node->IsNodeEnabled()
        || Node->IsDisplayAsDisabledForced()
        || Node->IsNodeUnrelated())
    {
        Color *= FLinearColor(0.5f, 0.5f, 0.5f, 0.4f);
    }
    else
    {
        Color.A = FadeCurve.GetLerp();
    }

    return FSlateColor(Color);
}

FLinearColor SHueGraphNodeK2Composite::GetNodeTitleTextColor() const
{
    return GetHueCompositeTitleTextColor().GetSpecifiedColor();
}

FSlateColor SHueGraphNodeK2Composite::GetNodeBodyColor() const
{
    return GetHueCompositeBodyColor();
}

TOptional<FSlateColor> SHueGraphNodeK2Composite::GetPinTextColor(
    const SGraphPin* InGraphPin) const
{
    const UK2Node_Composite* Node = GetHueNode();
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
    if (!FHueStyleResolver::ResolveColor(
        Node,
        EHueStyleChannel::BodyTextColor,
        Color))
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

TSharedPtr<SGraphPin> SHueGraphNodeK2Composite::CreatePinWidget(
    UEdGraphPin* Pin) const
{
    if (Pin && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
    {
        return SNew(SHueGraphPinExec, Pin);
    }

    return SGraphNodeK2Composite::CreatePinWidget(Pin);
}

FSlateColor SHueGraphNodeK2Composite::GetHueCompositeBodyColor() const
{
    const UK2Node_Composite* Node = GetHueNode();
    if (!Node)
    {
        return FSlateColor(FLinearColor::White);
    }

    FLinearColor Color;
    if (!FHueStyleResolver::ResolveColor(
        Node,
        EHueStyleChannel::BodyColor,
        Color))
    {
        return FSlateColor(FLinearColor::White);
    }

    if (!Node->IsNodeEnabled()
        || Node->IsDisplayAsDisabledForced()
        || Node->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return FSlateColor(Color);
}

FSlateColor SHueGraphNodeK2Composite::GetHueCompositeTitleTextColor() const
{
    const UK2Node_Composite* Node = GetHueNode();
    if (!Node)
    {
        return FSlateColor::UseForeground();
    }

    FLinearColor Color;
    if (!FHueStyleResolver::ResolveColor(
        Node,
        EHueStyleChannel::HeaderTextColor,
        Color))
    {
        return FSlateColor(Node->GetNodeTitleTextColor());
    }

    if (!Node->IsNodeEnabled()
        || Node->IsDisplayAsDisabledForced()
        || Node->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return FSlateColor(Color);
}

void SHueGraphNodeK2Composite::BindHueToNativeCompositeLayers()
{
    if (SBorder* BodyBorder =
        HueGraphNodeK2CompositePrivate::FindCompositeBodyBorder(*this))
    {
        BodyBorder->SetBorderBackgroundColor(
            TAttribute<FSlateColor>::Create(
                TAttribute<FSlateColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeK2Composite::GetHueCompositeBodyColor)));
    }

    if (InlineEditableText.IsValid())
    {
        InlineEditableText->SetColorAndOpacity(
            TAttribute<FLinearColor>::Create(
                TAttribute<FLinearColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeK2Composite::GetNodeTitleTextColor)));
    }

    if (SNodeTitle* NodeTitle =
        HueGraphNodeK2CompositePrivate::FindNodeTitle(*this))
    {
        NodeTitle->SetColorAndOpacity(
            TAttribute<FLinearColor>::Create(
                TAttribute<FLinearColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeK2Composite::GetNodeTitleTextColor)));
    }
}
