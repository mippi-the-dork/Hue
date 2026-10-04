// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "KismetNodes/SGraphNodeK2Default.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SGraphPin;
class SImage;
class SNodeTitle;
class STextBlock;
class UK2Node;

/**
 * Hue's minimally customized default K2 Slate node.
 * The historical class name is retained so existing source updates remain drop-in.
 *
 * This wrapper also handles Unreal's default compact K2 presentation. Compact
 * nodes have one visual fill rather than separate title/body regions, so Hue
 * maps Header Color to that fill with Body Color as a fallback.
 */
class SHueGraphNodeCallFunction : public SGraphNodeK2Default
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeCallFunction) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UK2Node* InNode);

    virtual void UpdateGraphNode() override;
    virtual FSlateColor GetNodeTitleColor() const override;
    virtual FLinearColor GetNodeTitleTextColor() const override;
    virtual FSlateColor GetNodeBodyColor() const override;
    virtual TOptional<FSlateColor> GetPinTextColor(const SGraphPin* InGraphPin) const override;
    virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

protected:
    virtual TSharedRef<SWidget> CreateTitleWidget(TSharedPtr<SNodeTitle> NodeTitle) override;

private:
    const UK2Node* GetHueNode() const;
    FSlateColor GetCompactFillColor() const;
    FSlateColor GetCompactTitleColor() const;
    void BindHueToCompactPresentation();
};
