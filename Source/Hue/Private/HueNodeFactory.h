// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"

class FHueNodeFactory : public FGraphPanelNodeFactory
{
public:
    virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};
