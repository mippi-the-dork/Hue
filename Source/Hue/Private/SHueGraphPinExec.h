// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "KismetPins/SGraphPinExec.h"

class SHueGraphPinExec : public SGraphPinExec
{
public:
    SLATE_BEGIN_ARGS(SHueGraphPinExec) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

    virtual FSlateColor GetPinColor() const override;
};
