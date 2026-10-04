// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "EdGraphUtilities.h"

class FHuePinFactory : public FGraphPanelPinFactory
{
public:
    virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override;
};
