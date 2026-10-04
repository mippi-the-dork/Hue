// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeDocumentation.h"

#include "EdGraph/EdGraphNode.h"
#include "HueStyleResolver.h"
#include "Layout/Children.h"
#include "Styling/AppStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Widgets/Text/STextBlock.h"

namespace HueDocumentationPrivate
{
    static SBorder* FindBodyBorder(SWidget& Widget)
    {
        if (Widget.GetType() == SBorder::StaticWidgetClass().GetWidgetType())
        {
            SBorder& Border = static_cast<SBorder&>(Widget);
            if (Border.GetBorderImage() == FAppStyle::GetBrush("Graph.Node.Body"))
            {
                return &Border;
            }
        }

        FChildren* WidgetChildren = Widget.GetChildren();
        if (!WidgetChildren)
        {
            return nullptr;
        }

        for (int32 ChildIndex = 0; ChildIndex < WidgetChildren->Num(); ++ChildIndex)
        {
            if (SBorder* Match = FindBodyBorder(
                WidgetChildren->GetChildAt(ChildIndex).Get()))
            {
                return Match;
            }
        }

        return nullptr;
    }

    static void BindTextBlocks(
        SWidget& Widget,
        TAttribute<FSlateColor> ColorAttribute)
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
            BindTextBlocks(
                WidgetChildren->GetChildAt(ChildIndex).Get(),
                ColorAttribute);
        }
    }
}

void SHueGraphNodeDocumentation::Construct(
    const FArguments& InArgs,
    UEdGraphNode* InNode)
{
    SGraphNodeDocumentation::Construct(
        SGraphNodeDocumentation::FArguments(),
        InNode);

    BindHueToNativeDocumentationLayers();
}

void SHueGraphNodeDocumentation::UpdateGraphNode()
{
    SGraphNodeDocumentation::UpdateGraphNode();
    BindHueToNativeDocumentationLayers();
}

FSlateColor SHueGraphNodeDocumentation::GetNodeTitleColor() const
{
    if (!GraphNode || GraphNode->IsDeprecated())
    {
        return SGraphNodeDocumentation::GetNodeTitleColor();
    }

    FLinearColor Color;
    if (!FHueStyleResolver::ResolveColor(
        GraphNode,
        EHueStyleChannel::HeaderColor,
        Color))
    {
        return SGraphNodeDocumentation::GetNodeTitleColor();
    }

    if (!GraphNode->IsNodeEnabled()
        || GraphNode->IsDisplayAsDisabledForced()
        || GraphNode->IsNodeUnrelated())
    {
        Color *= FLinearColor(0.5f, 0.5f, 0.5f, 0.4f);
    }
    else
    {
        Color.A = FadeCurve.GetLerp();
    }

    return FSlateColor(Color);
}

FLinearColor SHueGraphNodeDocumentation::GetNodeTitleTextColor() const
{
    FLinearColor Color;
    if (!GraphNode
        || !FHueStyleResolver::ResolveColor(
            GraphNode,
            EHueStyleChannel::HeaderTextColor,
            Color))
    {
        return SGraphNodeDocumentation::GetNodeTitleTextColor();
    }

    if (!GraphNode->IsNodeEnabled()
        || GraphNode->IsDisplayAsDisabledForced()
        || GraphNode->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return Color;
}

FSlateColor SHueGraphNodeDocumentation::GetNodeBodyColor() const
{
    FLinearColor Color;
    if (!GraphNode
        || !FHueStyleResolver::ResolveColor(
            GraphNode,
            EHueStyleChannel::BodyColor,
            Color))
    {
        return SGraphNodeDocumentation::GetNodeBodyColor();
    }

    if (!GraphNode->IsNodeEnabled()
        || GraphNode->IsDisplayAsDisabledForced()
        || GraphNode->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return FSlateColor(Color);
}

FSlateColor SHueGraphNodeDocumentation::GetDocumentationBodyTextColor() const
{
    FLinearColor Color;
    if (!GraphNode
        || !FHueStyleResolver::ResolveColor(
            GraphNode,
            EHueStyleChannel::BodyTextColor,
            Color))
    {
        return FSlateColor::UseForeground();
    }

    if (!GraphNode->IsNodeEnabled()
        || GraphNode->IsDisplayAsDisabledForced()
        || GraphNode->IsNodeUnrelated())
    {
        Color *= FLinearColor(1.0f, 1.0f, 1.0f, 0.5f);
    }

    return FSlateColor(Color);
}

void SHueGraphNodeDocumentation::BindHueToNativeDocumentationLayers()
{
    if (InlineEditableText.IsValid())
    {
        InlineEditableText->SetColorAndOpacity(
            TAttribute<FLinearColor>::Create(
                TAttribute<FLinearColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeDocumentation::GetNodeTitleTextColor)));
    }

    if (SBorder* BodyBorder = HueDocumentationPrivate::FindBodyBorder(*this))
    {
        BodyBorder->SetBorderBackgroundColor(
            TAttribute<FSlateColor>::Create(
                TAttribute<FSlateColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeDocumentation::GetNodeBodyColor)));

        // SGraphNodeDocumentation::ContentWidget is private in the installed
        // UE 5.8 build. Traverse the already-located native body subtree
        // instead, keeping Hue on public/protected Slate APIs.
        HueDocumentationPrivate::BindTextBlocks(
            *BodyBorder,
            TAttribute<FSlateColor>::Create(
                TAttribute<FSlateColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeDocumentation::GetDocumentationBodyTextColor)));
    }
}
