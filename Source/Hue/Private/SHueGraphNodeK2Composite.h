// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "KismetNodes/SGraphNodeK2Composite.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SGraphPin;
class SNodeTitle;
class UK2Node_Composite;

/**
 * Hue presentation for collapsed graph / Composite nodes.
 *
 * This derives from Unreal's public native composite renderer so the graph
 * preview tooltip and collapsed-graph behavior stay native.
 */
class SHueGraphNodeK2Composite : public SGraphNodeK2Composite
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeK2Composite) {}
    SLATE_END_ARGS()

    void Construct(
        const FArguments& InArgs,
        UK2Node_Composite* InNode);

    virtual void UpdateGraphNode() override;
    virtual FSlateColor GetNodeTitleColor() const override;
    virtual FLinearColor GetNodeTitleTextColor() const override;
    virtual FSlateColor GetNodeBodyColor() const override;
    virtual TOptional<FSlateColor> GetPinTextColor(
        const SGraphPin* InGraphPin) const override;
    virtual TSharedPtr<SGraphPin> CreatePinWidget(
        UEdGraphPin* Pin) const override;

private:
    const UK2Node_Composite* GetHueNode() const;

    FSlateColor GetHueCompositeBodyColor() const;
    FSlateColor GetHueCompositeTitleTextColor() const;

    void BindHueToNativeCompositeLayers();
};
