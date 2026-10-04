// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "KismetNodes/SGraphNodeK2Var.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SGraphPin;
class UK2Node;

/**
 * Hue presentation for nodes Unreal draws through SGraphNodeK2Var.
 *
 * The native variable widget builds a specialized body and color-spill layout.
 * Hue lets Unreal build that layout first, then binds only the two native image
 * layers that correspond to the body and title spill. If the expected layout
 * is not present, Hue leaves the native widget untouched rather than guessing.
 */
class SHueGraphNodeK2Var : public SGraphNodeK2Var
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeK2Var) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UK2Node* InNode);

    virtual void UpdateGraphNode() override;
    virtual FSlateColor GetNodeTitleColor() const override;
    virtual FLinearColor GetNodeTitleTextColor() const override;
    virtual FSlateColor GetNodeBodyColor() const override;
    virtual TOptional<FSlateColor> GetPinTextColor(const SGraphPin* InGraphPin) const override;
    virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

protected:
    virtual TSharedRef<SWidget> UpdateTitleWidget(
        FText InTitleText,
        TSharedPtr<SWidget> InTitleWidget,
        EHorizontalAlignment& InOutTitleHAlign,
        FMargin& InOutTitleMargin) const override;

private:
    const UK2Node* GetHueNode() const;

    FSlateColor GetHueVariableHeaderColor() const;
    FSlateColor GetHueVariableBodyColor() const;
    FSlateColor GetHueVariableTitleTextColor() const;
    FSlateColor GetHueVariableSubtitleTextColor() const;

    void BindHueToNativeVariableLayers();
    void BindHueToTitleText(const TSharedRef<SWidget>& TitleWidget) const;
};
