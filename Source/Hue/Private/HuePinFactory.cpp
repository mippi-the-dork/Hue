// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HuePinFactory.h"

#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "HueStyleResolver.h"
#include "SHueGraphPinExec.h"

TSharedPtr<SGraphPin> FHuePinFactory::CreatePin(UEdGraphPin* Pin) const
{
    if (!Pin
        || Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec
        || !FHueStyleResolver::IsSupportedNode(Pin->GetOwningNodeUnchecked()))
    {
        return nullptr;
    }

    return SNew(SHueGraphPinExec, Pin);
}
