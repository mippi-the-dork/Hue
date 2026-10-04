// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"

class SGraphNode;
class UK2Node;

/**
 * Applies Hue attributes to a native specialized K2 Slate widget without
 * replacing the widget or its controls.
 */
class FHueNativeNodeDecorator
{
public:
    static void Apply(const TSharedRef<SGraphNode>& NativeNode, UK2Node* Node);
};
