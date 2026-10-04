// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "SHueGraphNodeK2Var.h"

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

namespace HueGraphNodeK2VarPrivate
{
    static bool HasLeadingImageTriplet(SWidget& Widget)
    {
        FChildren* Children = Widget.GetChildren();
        if (!Children || Children->Num() < 3)
        {
            return false;
        }

        const FName ImageType = SImage::StaticWidgetClass().GetWidgetType();
        return Children->GetChildAt(0)->GetType() == ImageType
            && Children->GetChildAt(1)->GetType() == ImageType
            && Children->GetChildAt(2)->GetType() == ImageType;
    }

    static SWidget* FindNativeVariableOverlay(SWidget& Widget)
    {
        if (HasLeadingImageTriplet(Widget))
        {
            return &Widget;
        }

        FChildren* Children = Widget.GetChildren();
        if (!Children)
        {
            return nullptr;
        }

        for (int32 ChildIndex = 0; ChildIndex < Children->Num(); ++ChildIndex)
        {
            TSharedRef<SWidget> Child = Children->GetChildAt(ChildIndex);
            if (SWidget* Match = FindNativeVariableOverlay(Child.Get()))
            {
                return Match;
            }
        }

        return nullptr;
    }

    static void CollectTextBlocks(
        SWidget& Widget,
        TArray<STextBlock*>& OutTextBlocks)
    {
        if (Widget.GetType() == STextBlock::StaticWidgetClass().GetWidgetType())
        {
            OutTextBlocks.Add(static_cast<STextBlock*>(&Widget));
        }

        FChildren* Children = Widget.GetChildren();
        if (!Children)
        {
            return;
        }

        for (int32 ChildIndex = 0; ChildIndex < Children->Num(); ++ChildIndex)
        {
            CollectTextBlocks(Children->GetChildAt(ChildIndex).Get(), OutTextBlocks);
        }
    }
}

void SHueGraphNodeK2Var::Construct(
    const FArguments& InArgs,
    UK2Node* InNode)
{
    SGraphNodeK2Var::Construct(SGraphNodeK2Var::FArguments(), InNode);
}

const UK2Node* SHueGraphNodeK2Var::GetHueNode() const
{
    return Cast<UK2Node>(GraphNode);
}

void SHueGraphNodeK2Var::UpdateGraphNode()
{
    // Let Unreal construct the complete specialized variable presentation.
    SGraphNodeK2Var::UpdateGraphNode();

    // Bind only the well-known body and color-spill image layers. If Unreal's
    // layout changes, this helper fails closed and leaves the native visuals.
    BindHueToNativeVariableLayers();
}

FSlateColor SHueGraphNodeK2Var::GetNodeTitleColor() const
{
    return GetHueVariableHeaderColor();
}

FLinearColor SHueGraphNodeK2Var::GetNodeTitleTextColor() const
{
    return GetHueVariableTitleTextColor().GetSpecifiedColor();
}

FSlateColor SHueGraphNodeK2Var::GetNodeBodyColor() const
{
    return GetHueVariableBodyColor();
}

TOptional<FSlateColor> SHueGraphNodeK2Var::GetPinTextColor(
    const SGraphPin* InGraphPin) const
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

TSharedPtr<SGraphPin> SHueGraphNodeK2Var::CreatePinWidget(
    UEdGraphPin* Pin) const
{
    if (Pin && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
    {
        return SNew(SHueGraphPinExec, Pin);
    }

    return SGraphNodeK2Var::CreatePinWidget(Pin);
}

TSharedRef<SWidget> SHueGraphNodeK2Var::UpdateTitleWidget(
    FText InTitleText,
    TSharedPtr<SWidget> InTitleWidget,
    EHorizontalAlignment& InOutTitleHAlign,
    FMargin& InOutTitleMargin) const
{
    TSharedRef<SWidget> NativeTitle =
        SGraphNodeK2Var::UpdateTitleWidget(
            InTitleText,
            InTitleWidget,
            InOutTitleHAlign,
            InOutTitleMargin);

    BindHueToTitleText(NativeTitle);
    return NativeTitle;
}

FSlateColor SHueGraphNodeK2Var::GetHueVariableHeaderColor() const
{
    const UK2Node* Node = GetHueNode();
    if (!Node)
    {
        return FSlateColor(FLinearColor::White);
    }

    FLinearColor Color;
    if (!FHueStyleResolver::ResolveColor(
        Node,
        EHueStyleChannel::HeaderColor,
        Color))
    {
        return FSlateColor(Node->GetNodeTitleColor());
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

FSlateColor SHueGraphNodeK2Var::GetHueVariableBodyColor() const
{
    const UK2Node* Node = GetHueNode();
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
        // SGraphNodeK2Var draws its native body brush without an extra tint.
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

FSlateColor SHueGraphNodeK2Var::GetHueVariableTitleTextColor() const
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

FSlateColor SHueGraphNodeK2Var::GetHueVariableSubtitleTextColor() const
{
    FLinearColor Color = GetHueVariableTitleTextColor().GetSpecifiedColor();
    Color.A *= 0.6f;
    return FSlateColor(Color);
}

void SHueGraphNodeK2Var::BindHueToNativeVariableLayers()
{
    SWidget* NativeOverlay =
        HueGraphNodeK2VarPrivate::FindNativeVariableOverlay(*this);

    if (!NativeOverlay)
    {
        return;
    }

    FChildren* OverlayChildren = NativeOverlay->GetChildren();
    if (!OverlayChildren || OverlayChildren->Num() < 3)
    {
        return;
    }

    const FName ImageType = SImage::StaticWidgetClass().GetWidgetType();
    if (OverlayChildren->GetChildAt(0)->GetType() != ImageType
        || OverlayChildren->GetChildAt(1)->GetType() != ImageType)
    {
        return;
    }

    SImage& BodyImage =
        static_cast<SImage&>(OverlayChildren->GetChildAt(0).Get());
    SImage& HeaderSpill =
        static_cast<SImage&>(OverlayChildren->GetChildAt(1).Get());

    BodyImage.SetColorAndOpacity(
        TAttribute<FSlateColor>::Create(
            TAttribute<FSlateColor>::FGetter::CreateSP(
                this,
                &SHueGraphNodeK2Var::GetHueVariableBodyColor)));

    HeaderSpill.SetColorAndOpacity(
        TAttribute<FSlateColor>::Create(
            TAttribute<FSlateColor>::FGetter::CreateSP(
                this,
                &SHueGraphNodeK2Var::GetHueVariableHeaderColor)));
}

void SHueGraphNodeK2Var::BindHueToTitleText(
    const TSharedRef<SWidget>& TitleWidget) const
{
    TArray<STextBlock*> TextBlocks;
    HueGraphNodeK2VarPrivate::CollectTextBlocks(
        TitleWidget.Get(),
        TextBlocks);

    if (TextBlocks.Num() == 0)
    {
        return;
    }

    TextBlocks[0]->SetColorAndOpacity(
        TAttribute<FSlateColor>::Create(
            TAttribute<FSlateColor>::FGetter::CreateSP(
                this,
                &SHueGraphNodeK2Var::GetHueVariableTitleTextColor)));

    for (int32 Index = 1; Index < TextBlocks.Num(); ++Index)
    {
        TextBlocks[Index]->SetColorAndOpacity(
            TAttribute<FSlateColor>::Create(
                TAttribute<FSlateColor>::FGetter::CreateSP(
                    this,
                    &SHueGraphNodeK2Var::GetHueVariableSubtitleTextColor)));
    }
}
