// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "SGraphNodeDocumentation.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UEdGraphNode;

/**
 * Hue-compatible Blueprint Documentation node presentation.
 *
 * Documentation nodes are not UK2Node objects, so Hue handles their native
 * public SGraphNodeDocumentation presentation directly while preserving its
 * resizing, documentation-page, scrolling, and link behavior.
 */
class SHueGraphNodeDocumentation : public SGraphNodeDocumentation
{
public:
    SLATE_BEGIN_ARGS(SHueGraphNodeDocumentation) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

    virtual void UpdateGraphNode() override;
    virtual FSlateColor GetNodeTitleColor() const override;
    virtual FLinearColor GetNodeTitleTextColor() const override;
    virtual FSlateColor GetNodeBodyColor() const override;

private:
    FSlateColor GetDocumentationBodyTextColor() const;
    void BindHueToNativeDocumentationLayers();
};
