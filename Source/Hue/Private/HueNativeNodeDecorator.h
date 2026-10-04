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
    /**
     * Applies Hue attributes to recognizable native GraphEditor layers.
     * Returns true only when both a compatible header and body surface were
     * found, which is the minimum needed for Hue to claim full node support.
     */
    static bool Apply(const TSharedRef<SGraphNode>& NativeNode, UK2Node* Node);

    /**
     * Applies Hue to the native widget currently representing a node that owns
     * its CreateVisualWidget path. Returns false until Unreal has finished
     * assigning the displayed widget, or when the native widget does not expose
     * a compatible standard header/body presentation for Hue to decorate.
     */
    static bool ApplyToDisplayedNode(UK2Node* Node);
};
