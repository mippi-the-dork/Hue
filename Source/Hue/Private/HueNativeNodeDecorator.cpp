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

    static void DecorateWidgetTree(
        SWidget& Widget,
        const TWeakObjectPtr<UK2Node>& WeakNode)
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
            SImage& BodyImage = static_cast<SImage&>(
                WidgetChildren->GetChildAt(0).Get());

            BodyImage.SetColorAndOpacity(
                TAttribute<FSlateColor>::Create(
                    TAttribute<FSlateColor>::FGetter::CreateLambda(
                        [WeakNode]()
                        {
                            return ResolveSlateColor(
                                WeakNode,
                                EHueStyleChannel::BodyColor,
                                GetNativeBodyFallback(WeakNode));
                        })));
        }

        if (Widget.GetType() == BorderType)
        {
            SBorder& Border = static_cast<SBorder&>(Widget);
            if (Border.GetBorderImage() == FAppStyle::GetBrush("Graph.Node.ColorSpill"))
            {
                const TAttribute<FSlateColor> HeaderAttribute =
                    TAttribute<FSlateColor>::Create(
                        TAttribute<FSlateColor>::FGetter::CreateLambda(
                            [WeakNode]()
                            {
                                return ResolveSlateColor(
                                    WeakNode,
                                    EHueStyleChannel::HeaderColor,
                                    GetNativeHeaderFallback(WeakNode));
                            }));

                Border.SetBorderBackgroundColor(HeaderAttribute);

                const TAttribute<FSlateColor> HeaderTextAttribute =
                    TAttribute<FSlateColor>::Create(
                        TAttribute<FSlateColor>::FGetter::CreateLambda(
                            [WeakNode]()
                            {
                                return ResolveSlateColor(
                                    WeakNode,
                                    EHueStyleChannel::HeaderTextColor,
                                    GetNativeHeaderTextFallback(WeakNode));
                            }));

                BindTextBlocks(Border.GetContent().Get(), HeaderTextAttribute);
            }
        }
        FChildren* Children = Widget.GetChildren();
        if (!Children)
        {
            return;
        }

        for (int32 ChildIndex = 0; ChildIndex < Children->Num(); ++ChildIndex)
        {
            DecorateWidgetTree(Children->GetChildAt(ChildIndex).Get(), WeakNode);
        }
    }
}

void FHueNativeNodeDecorator::Apply(
    const TSharedRef<SGraphNode>& NativeNode,
    UK2Node* Node)
{
    if (!Node)
    {
        return;
    }

    const TWeakObjectPtr<UK2Node> WeakNode(Node);
    HueNativeNodeDecoratorPrivate::DecorateWidgetTree(NativeNode.Get(), WeakNode);

    TArray<TSharedRef<SWidget>> PinWidgets;
    NativeNode->GetPins(PinWidgets);
    for (const TSharedRef<SWidget>& PinWidget : PinWidgets)
    {
        TSharedRef<SGraphPin> GraphPin = StaticCastSharedRef<SGraphPin>(PinWidget);
        HueNativeNodeDecoratorPrivate::BindPinLabel(GraphPin.Get(), WeakNode);
    }
}
