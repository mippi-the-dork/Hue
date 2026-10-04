// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueNativeNodeDecorator.h"

#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "HueStyleResolver.h"
#include "K2Node.h"
#include "Layout/Children.h"
#include "SGraphNode.h"
#include "SGraphPin.h"
#include "Styling/AppStyle.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

namespace HueNativeNodeDecoratorPrivate
{
    static FSlateColor ResolveSlateColor(
        const TWeakObjectPtr<UK2Node>& WeakNode,
        EHueStyleChannel Channel,
        const FSlateColor& Fallback)
    {
        const UK2Node* Node = WeakNode.Get();
        if (!Node)
        {
            return Fallback;
        }

        FLinearColor Color;
        if (!FHueStyleResolver::ResolveColor(Node, Channel, Color))
        {
            return Fallback;
        }

        if (!Node->IsNodeEnabled()
            || Node->IsDisplayAsDisabledForced()
            || Node->IsNodeUnrelated())
        {
            const float AlphaScale =
                Channel == EHueStyleChannel::HeaderColor ? 0.4f : 0.5f;
            Color *= FLinearColor(1.0f, 1.0f, 1.0f, AlphaScale);
        }

        return FSlateColor(Color);
    }

    static FSlateColor GetNativeHeaderFallback(
        const TWeakObjectPtr<UK2Node>& WeakNode)
    {
        if (const UK2Node* Node = WeakNode.Get())
        {
            return FSlateColor(Node->GetNodeTitleColor());
        }
        return FSlateColor(FLinearColor::White);
    }

    static FSlateColor GetNativeBodyFallback(
        const TWeakObjectPtr<UK2Node>& WeakNode)
    {
        if (const UK2Node* Node = WeakNode.Get())
        {
            return FSlateColor(Node->GetNodeBodyTintColor());
        }
        return FSlateColor(FLinearColor::White);
    }

    static FSlateColor GetNativeHeaderTextFallback(
        const TWeakObjectPtr<UK2Node>& WeakNode)
    {
        if (const UK2Node* Node = WeakNode.Get())
        {
            return FSlateColor(Node->GetNodeTitleTextColor());
        }
        return FSlateColor::UseForeground();
    }

    static void BindTextBlocks(
        SWidget& Widget,
        const TAttribute<FSlateColor>& ColorAttribute)
    {
        if (Widget.GetType() == STextBlock::StaticWidgetClass().GetWidgetType())
        {
            static_cast<STextBlock&>(Widget).SetColorAndOpacity(ColorAttribute);
        }

        FChildren* WidgetChildren = Widget.GetChildren();
        if (!WidgetChildren)
        {
            return;
        }

        for (int32 ChildIndex = 0; ChildIndex < WidgetChildren->Num(); ++ChildIndex)
        {
            BindTextBlocks(WidgetChildren->GetChildAt(ChildIndex).Get(), ColorAttribute);
        }
    }

    static void BindPinLabel(
        SGraphPin& PinWidget,
        const TWeakObjectPtr<UK2Node>& WeakNode)
    {
        UEdGraphPin* Pin = PinWidget.GetPinObj();
        if (!Pin || !Pin->GetSchema())
        {
            return;
        }

        const FText ExpectedLabel = Pin->GetSchema()->GetPinDisplayName(Pin);
        if (ExpectedLabel.IsEmpty())
        {
            return;
        }

        const TAttribute<FSlateColor> BodyTextAttribute =
            TAttribute<FSlateColor>::Create(
                TAttribute<FSlateColor>::FGetter::CreateLambda(
                    [WeakNode]()
                    {
                        return ResolveSlateColor(
                            WeakNode,
                            EHueStyleChannel::BodyTextColor,
                            FSlateColor::UseForeground());
                    }));

        struct FLabelBinder
        {
            static bool Bind(
                SWidget& Widget,
                const FText& Label,
                const TAttribute<FSlateColor>& ColorAttribute)
            {
                if (Widget.GetType() == STextBlock::StaticWidgetClass().GetWidgetType())
                {
                    STextBlock& TextBlock = static_cast<STextBlock&>(Widget);
                    if (TextBlock.GetText().EqualTo(Label))
                    {
                        TextBlock.SetColorAndOpacity(ColorAttribute);
                        return true;
                    }
                }

                FChildren* Children = Widget.GetChildren();
                if (!Children)
                {
                    return false;
                }

                for (int32 Index = 0; Index < Children->Num(); ++Index)
                {
                    if (Bind(Children->GetChildAt(Index).Get(), Label, ColorAttribute))
                    {
                        return true;
                    }
                }

                return false;
            }
        };

        FLabelBinder::Bind(PinWidget, ExpectedLabel, BodyTextAttribute);
    }

    static bool ContainsTitleColorSpill(SWidget& Widget)
    {
        if (Widget.GetType() == SBorder::StaticWidgetClass().GetWidgetType())
        {
            SBorder& Border = static_cast<SBorder&>(Widget);
            if (Border.GetBorderImage() == FAppStyle::GetBrush("Graph.Node.ColorSpill"))
            {
                return true;
            }
        }

        FChildren* Children = Widget.GetChildren();
        if (!Children)
        {
            return false;
        }

        for (int32 Index = 0; Index < Children->Num(); ++Index)
        {
            if (ContainsTitleColorSpill(Children->GetChildAt(Index).Get()))
            {
                return true;
            }
        }

        return false;
    }

    static void CollectCompatibleSurfaces(
        SWidget& Widget,
        TArray<SBorder*>& OutHeaderBorders,
        TArray<SImage*>& OutBodyImages)
    {
        const FName BorderType = SBorder::StaticWidgetClass().GetWidgetType();
        const FName ImageType = SImage::StaticWidgetClass().GetWidgetType();

        // Standard GraphEditor nodes place the node body in a two-slot overlay:
        // body image first, then the inner vertical content containing the title.
        // Match that structure instead of reading SImage's protected brush data.
        FChildren* WidgetChildren = Widget.GetChildren();
        if (WidgetChildren
            && WidgetChildren->Num() == 2
            && WidgetChildren->GetChildAt(0)->GetType() == ImageType
            && ContainsTitleColorSpill(WidgetChildren->GetChildAt(1).Get()))
        {
            OutBodyImages.AddUnique(static_cast<SImage*>(
                &WidgetChildren->GetChildAt(0).Get()));
        }

        if (Widget.GetType() == BorderType)
        {
            SBorder& Border = static_cast<SBorder&>(Widget);
            if (Border.GetBorderImage() == FAppStyle::GetBrush("Graph.Node.ColorSpill"))
            {
                OutHeaderBorders.AddUnique(&Border);
            }
        }

        if (!WidgetChildren)
        {
            return;
        }

        for (int32 ChildIndex = 0; ChildIndex < WidgetChildren->Num(); ++ChildIndex)
        {
            CollectCompatibleSurfaces(
                WidgetChildren->GetChildAt(ChildIndex).Get(),
                OutHeaderBorders,
                OutBodyImages);
        }
    }

}

bool FHueNativeNodeDecorator::Apply(
    const TSharedRef<SGraphNode>& NativeNode,
    UK2Node* Node)
{
    if (!Node)
    {
        return false;
    }

    TArray<SBorder*> HeaderBorders;
    TArray<SImage*> BodyImages;
    HueNativeNodeDecoratorPrivate::CollectCompatibleSurfaces(
        NativeNode.Get(),
        HeaderBorders,
        BodyImages);

    // Hue's four-channel contract assumes a recognizable title and body
    // surface. Detect first and mutate second so an incompatible custom widget
    // is left completely untouched rather than partially decorated.
    if (HeaderBorders.IsEmpty() || BodyImages.IsEmpty())
    {
        return false;
    }

    const TWeakObjectPtr<UK2Node> WeakNode(Node);

    const TAttribute<FSlateColor> BodyAttribute =
        TAttribute<FSlateColor>::Create(
            TAttribute<FSlateColor>::FGetter::CreateLambda(
                [WeakNode]()
                {
                    return HueNativeNodeDecoratorPrivate::ResolveSlateColor(
                        WeakNode,
                        EHueStyleChannel::BodyColor,
                        HueNativeNodeDecoratorPrivate::GetNativeBodyFallback(WeakNode));
                }));

    for (SImage* BodyImage : BodyImages)
    {
        if (BodyImage)
        {
            BodyImage->SetColorAndOpacity(BodyAttribute);
        }
    }

    const TAttribute<FSlateColor> HeaderAttribute =
        TAttribute<FSlateColor>::Create(
            TAttribute<FSlateColor>::FGetter::CreateLambda(
                [WeakNode]()
                {
                    return HueNativeNodeDecoratorPrivate::ResolveSlateColor(
                        WeakNode,
                        EHueStyleChannel::HeaderColor,
                        HueNativeNodeDecoratorPrivate::GetNativeHeaderFallback(WeakNode));
                }));

    const TAttribute<FSlateColor> HeaderTextAttribute =
        TAttribute<FSlateColor>::Create(
            TAttribute<FSlateColor>::FGetter::CreateLambda(
                [WeakNode]()
                {
                    return HueNativeNodeDecoratorPrivate::ResolveSlateColor(
                        WeakNode,
                        EHueStyleChannel::HeaderTextColor,
                        HueNativeNodeDecoratorPrivate::GetNativeHeaderTextFallback(WeakNode));
                }));

    for (SBorder* HeaderBorder : HeaderBorders)
    {
        if (!HeaderBorder)
        {
            continue;
        }

        HeaderBorder->SetBorderBackgroundColor(HeaderAttribute);
        HueNativeNodeDecoratorPrivate::BindTextBlocks(
            HeaderBorder->GetContent().Get(),
            HeaderTextAttribute);
    }

    TArray<TSharedRef<SWidget>> PinWidgets;
    NativeNode->GetPins(PinWidgets);
    for (const TSharedRef<SWidget>& PinWidget : PinWidgets)
    {
        TSharedRef<SGraphPin> GraphPin = StaticCastSharedRef<SGraphPin>(PinWidget);
        HueNativeNodeDecoratorPrivate::BindPinLabel(GraphPin.Get(), WeakNode);
    }

    return true;
}

bool FHueNativeNodeDecorator::ApplyToDisplayedNode(UK2Node* Node)
{
    if (!Node)
    {
        return false;
    }

    // UEdGraphNode::DEPRECATED_NodeWidget is the only public bridge exposed by
    // Unreal for reaching a node-owned visual widget after CreateVisualWidget
    // bypasses registered graph factories. Hue uses it only as a compatibility
    // fallback for nodes that never reached Hue's registered visual factory.
    TSharedPtr<SGraphNode> NativeNode = Node->DEPRECATED_NodeWidget.Pin();
    if (!NativeNode.IsValid())
    {
        return false;
    }

    return Apply(NativeNode.ToSharedRef(), Node);
}
