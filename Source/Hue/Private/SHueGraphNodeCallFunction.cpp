// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeCallFunction.h"

#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "HueStyleResolver.h"
#include "K2Node.h"
#include "Layout/Children.h"
#include "SGraphPin.h"
#include "SHueGraphPinExec.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

namespace HueGraphNodeDefaultPrivate
{
    static SImage* FindCompactBodyImage(SWidget& Widget)
    {
        FChildren* WidgetChildren = Widget.GetChildren();
        if (WidgetChildren && WidgetChildren->Num() >= 2)
        {
            const FName ImageType = SImage::StaticWidgetClass().GetWidgetType();
            if (WidgetChildren->GetChildAt(0)->GetType() == ImageType
                && WidgetChildren->GetChildAt(1)->GetType() == ImageType)
            {
                // Unreal's compact K2 layout starts its central presentation
                // with Graph.VarNode.Body followed by Graph.VarNode.Gloss.
                return static_cast<SImage*>(&WidgetChildren->GetChildAt(0).Get());
            }
        }

        if (!WidgetChildren)
        {
            return nullptr;
        }

        for (int32 ChildIndex = 0; ChildIndex < WidgetChildren->Num(); ++ChildIndex)
        {
            if (SImage* Match = FindCompactBodyImage(
                WidgetChildren->GetChildAt(ChildIndex).Get()))
            {
                return Match;
            }
        }

        return nullptr;
    }

    static STextBlock* FindFirstTextBlock(SWidget& Widget)
    {
        if (Widget.GetType() == STextBlock::StaticWidgetClass().GetWidgetType())
        {
            return static_cast<STextBlock*>(&Widget);
        }

        FChildren* WidgetChildren = Widget.GetChildren();
        if (!WidgetChildren)
        {
            return nullptr;
        }

        for (int32 ChildIndex = 0; ChildIndex < WidgetChildren->Num(); ++ChildIndex)
        {
            if (STextBlock* Match = FindFirstTextBlock(
                WidgetChildren->GetChildAt(ChildIndex).Get()))
            {
                return Match;
            }
        }

        return nullptr;
    }
}

void SHueGraphNodeCallFunction::Construct(const FArguments& InArgs, UK2Node* InNode)
{
    SGraphNodeK2Default::Construct(SGraphNodeK2Default::FArguments(), InNode);
}

const UK2Node* SHueGraphNodeCallFunction::GetHueNode() const
{
    return Cast<UK2Node>(GraphNode);
}

void SHueGraphNodeCallFunction::UpdateGraphNode()
{
    SGraphNodeK2Default::UpdateGraphNode();

    const UK2Node* Node = GetHueNode();
    if (Node && Node->ShouldDrawCompact())
    {
        BindHueToCompactPresentation();
    }
}

FSlateColor SHueGraphNodeCallFunction::GetNodeTitleColor() const
{
    const UK2Node* Node = GetHueNode();
    if (!Node || Node->IsDeprecated())
    {
        return SGraphNodeK2Default::GetNodeTitleColor();
    }

    FLinearColor Color;
    if (!FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::HeaderColor, Color))
    {
        return SGraphNodeK2Default::GetNodeTitleColor();
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

FLinearColor SHueGraphNodeCallFunction::GetNodeTitleTextColor() const
{
    const UK2Node* Node = GetHueNode();
    FLinearColor Color;
    if (!Node || !FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::HeaderTextColor, Color))
    {
        return SGraphNodeK2Default::GetNodeTitleTextColor();
    }

    if (!Node->IsNodeEnabled() || Node->IsDisplayAsDisabledForced() || Node->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }
    return Color;
}

FSlateColor SHueGraphNodeCallFunction::GetNodeBodyColor() const
{
    const UK2Node* Node = GetHueNode();
    FLinearColor Color;
    if (!Node || !FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::BodyColor, Color))
    {
        return SGraphNodeK2Default::GetNodeBodyColor();
    }

    if (!Node->IsNodeEnabled() || Node->IsDisplayAsDisabledForced() || Node->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }
    return FSlateColor(Color);
}

TOptional<FSlateColor> SHueGraphNodeCallFunction::GetPinTextColor(const SGraphPin* InGraphPin) const
{
    const UK2Node* Node = GetHueNode();
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

TSharedPtr<SGraphPin> SHueGraphNodeCallFunction::CreatePinWidget(UEdGraphPin* Pin) const
{
    if (Pin && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
    {
        return SNew(SHueGraphPinExec, Pin);
    }
    return SGraphNodeK2Default::CreatePinWidget(Pin);
}

TSharedRef<SWidget> SHueGraphNodeCallFunction::CreateTitleWidget(TSharedPtr<SNodeTitle> NodeTitle)
{
    TSharedRef<SWidget> MainTitle = SGraphNodeK2Default::CreateTitleWidget(NodeTitle);

    // SNodeTitle owns only the secondary lines once the native graph widget
    // separates out the main title. Tinting the compound widget therefore lets
    // "Target is ..." and other subtitle lines follow Header Text Color while
    // preserving Unreal's native subdued alpha/font treatment.
    if (NodeTitle.IsValid())
    {
        NodeTitle->SetColorAndOpacity(
            TAttribute<FLinearColor>::Create(
                TAttribute<FLinearColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeCallFunction::GetNodeTitleTextColor)));
    }

    return MainTitle;
}

FSlateColor SHueGraphNodeCallFunction::GetCompactFillColor() const
{
    const UK2Node* Node = GetHueNode();
    if (!Node)
    {
        return FSlateColor(FLinearColor::White);
    }

    FLinearColor Color;

    // Compact nodes have a single central fill rather than distinct header and
    // body regions. Header Color is the primary compact fill. Body Color is a
    // fallback so body-only styles from existing Hue data remain visible.
    if (!FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::HeaderColor, Color)
        && !FHueStyleResolver::ResolveColor(Node, EHueStyleChannel::BodyColor, Color))
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

FSlateColor SHueGraphNodeCallFunction::GetCompactTitleColor() const
{
    const UK2Node* Node = GetHueNode();
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

void SHueGraphNodeCallFunction::BindHueToCompactPresentation()
{
    if (SImage* BodyImage = HueGraphNodeDefaultPrivate::FindCompactBodyImage(*this))
    {
        BodyImage->SetColorAndOpacity(
            TAttribute<FSlateColor>::Create(
                TAttribute<FSlateColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeCallFunction::GetCompactFillColor)));
    }

    // Unreal creates the compact title before the pin rows. Pin labels remain
    // governed by GetPinTextColor on their native SGraphPin widgets.
    if (STextBlock* TitleText = HueGraphNodeDefaultPrivate::FindFirstTextBlock(*this))
    {
        TitleText->SetColorAndOpacity(
            TAttribute<FSlateColor>::Create(
                TAttribute<FSlateColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeCallFunction::GetCompactTitleColor)));
    }
}
