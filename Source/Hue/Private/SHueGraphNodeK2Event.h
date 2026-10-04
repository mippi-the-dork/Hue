// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "KismetNodes/SGraphNodeK2Event.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SGraphPin;
class SNodeTitle;
class UK2Node_Event;

/**
 * Hue presentation for Blueprint event nodes.
 * Inherits Unreal's native event widget so delegate-output pin placement,
 * low-detail title behavior, and event-specific presentation remain intact.
 */
class SHueGraphNodeK2Event : public SGraphNodeK2Event
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeK2Event) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UK2Node_Event* InNode);

    virtual FSlateColor GetNodeTitleColor() const override;
    virtual FLinearColor GetNodeTitleTextColor() const override;
    virtual FSlateColor GetNodeBodyColor() const override;
    virtual TOptional<FSlateColor> GetPinTextColor(const SGraphPin* InGraphPin) const override;
    virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

protected:
    virtual TSharedRef<SWidget> CreateTitleWidget(TSharedPtr<SNodeTitle> NodeTitle) override;

private:
    const UK2Node_Event* GetHueNode() const;
};
