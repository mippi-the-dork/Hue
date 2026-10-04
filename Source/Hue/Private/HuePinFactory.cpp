// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HuePinFactory.h"

#include "Containers/Ticker.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "HueNativeNodeDecorator.h"
#include "HueStyleResolver.h"
#include "K2Node.h"
#include "SHueGraphPinExec.h"

namespace HuePinFactoryPrivate
{
    static TSet<TWeakObjectPtr<UK2Node>> PendingSelfVisualNodes;

    static void QueueSelfVisualBridge(UK2Node* Node)
    {
        if (!Node || !FHueStyleResolver::UsesSelfVisualBridge(Node))
        {
            return;
        }

        const TWeakObjectPtr<UK2Node> WeakNode(Node);
        if (PendingSelfVisualNodes.Contains(WeakNode))
        {
            return;
        }

        PendingSelfVisualNodes.Add(WeakNode);

        // A node-owned CreateVisualWidget() runs before registered graph node
        // factories, so Hue never gets the finished SGraphNode during the
        // normal node-factory pass. Pin creation still goes through the public
        // pin factory, which gives us a safe construction-time hook. Defer to
        // the next frame so SGraphPanel has assigned DEPRECATED_NodeWidget,
        // then decorate Unreal's exact native widget instead of replacing it.
        FTSTicker::GetCoreTicker().AddTicker(
            TEXT("Hue.SelfVisualNodeBridge"),
            0.0f,
            [WeakNode, Attempts = 0](float) mutable
            {
                UK2Node* LiveNode = WeakNode.Get();
                if (!LiveNode)
                {
                    PendingSelfVisualNodes.Remove(WeakNode);
                    return false;
                }

                if (FHueNativeNodeDecorator::ApplyToDisplayedNode(LiveNode))
                {
                    FHueStyleResolver::MarkVisualSupport(LiveNode);
                    PendingSelfVisualNodes.Remove(WeakNode);
                    return false;
                }

                // Normally the displayed widget is available on the first
                // deferred tick. A few retries cover Slate/editor startup
                // ordering without leaving a permanent polling task behind.
                ++Attempts;
                if (Attempts >= 5)
                {
                    PendingSelfVisualNodes.Remove(WeakNode);
                    return false;
                }

                return true;
            });
    }
}

TSharedPtr<SGraphPin> FHuePinFactory::CreatePin(UEdGraphPin* Pin) const
{
    if (!Pin || Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Exec)
    {
        return nullptr;
    }

    UK2Node* Node = Cast<UK2Node>(Pin->GetOwningNodeUnchecked());
    if (!Node)
    {
        return nullptr;
    }

    const bool bSelfVisualBridge = FHueStyleResolver::UsesSelfVisualBridge(Node);

    // Normal nodes are marked visually supported by FHueNodeFactory before
    // their pins are constructed. Node-owned visual widgets never reach that
    // factory, so the dedicated self-visual bridge is the one exception.
    if (!FHueStyleResolver::IsSupportedNode(Node) && !bSelfVisualBridge)
    {
        return nullptr;
    }

    if (bSelfVisualBridge)
    {
        HuePinFactoryPrivate::QueueSelfVisualBridge(Node);
    }

    return SNew(SHueGraphPinExec, Pin);
}
