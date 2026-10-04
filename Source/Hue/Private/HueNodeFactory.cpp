// Copyright Mippithedork 2026, Inc. All Rights Reserved.

#include "HueNodeFactory.h"

#include "HueNativeNodeDecorator.h"
#include "HueStyleResolver.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "K2Node_CallMaterialParameterCollectionFunction.h"
#include "K2Node_Composite.h"
#include "K2Node_Copy.h"
#include "K2Node_CreateDelegate.h"
#include "K2Node_Event.h"
#include "K2Node_FormatText.h"
#include "K2Node_MakeStruct.h"
#include "K2Node_PromotableOperator.h"
#include "K2Node_SpawnActor.h"
#include "K2Node_SpawnActorFromClass.h"
#include "K2Node_Switch.h"
#include "K2Node_Timeline.h"
#include "NodeFactory.h"
#include "SHueGraphNodeCallFunction.h"
#include "SHueGraphNodeFormatText.h"
#include "SHueGraphNodeK2Composite.h"
#include "SHueGraphNodeK2Event.h"
#include "SHueGraphNodeK2Operator.h"
#include "SHueGraphNodeK2Sequence.h"
#include "SHueGraphNodeK2Switch.h"
#include "SHueGraphNodeK2Timeline.h"
#include "SHueGraphNodeK2Var.h"

namespace HueNodeFactoryPrivate
{
    // FNodeFactory calls registered visual factories before its built-in K2
    // fallbacks. Hue uses one guarded re-entry for specialized renderers whose
    // Slate classes are private to GraphEditor. During that re-entry Hue must
    // decline the node so Unreal can reach its own specialized renderer.
    static thread_local bool bCreatingNativeSpecializedWidget = false;

    struct FNativeFactoryGuard
    {
        FNativeFactoryGuard()
        {
            bCreatingNativeSpecializedWidget = true;
        }

        ~FNativeFactoryGuard()
        {
            bCreatingNativeSpecializedWidget = false;
        }
    };

    static bool UsesNativeSpecializedBridge(const UK2Node* Node)
    {
        return Node
            && (Node->IsA<UK2Node_SpawnActor>()
                || Node->IsA<UK2Node_SpawnActorFromClass>()
                || Node->IsA<UK2Node_CreateDelegate>()
                || Node->IsA<UK2Node_CallMaterialParameterCollectionFunction>()
                || Node->IsA<UK2Node_MakeStruct>()
                || Node->IsA<UK2Node_Copy>());
    }

    static TSharedPtr<SGraphNode> CreateNativeSpecializedWidget(UK2Node* Node)
    {
        if (!Node)
        {
            return nullptr;
        }

        FNativeFactoryGuard Guard;
        TSharedPtr<SGraphNode> NativeWidget = FNodeFactory::CreateNodeWidget(Node);
        if (!NativeWidget.IsValid()
            || !FHueNativeNodeDecorator::Apply(NativeWidget.ToSharedRef(), Node))
        {
            // Hue marks nodes provisionally before native construction so its
            // exec-pin factory can participate. If the finished native widget
            // does not expose a compatible full presentation, retract that
            // claim. The Hue exec pin then falls back to Unreal's native color.
            FHueStyleResolver::UnmarkVisualSupport(Node);
        }
        return NativeWidget;
    }
}

TSharedPtr<SGraphNode> FHueNodeFactory::CreateNode(UEdGraphNode* Node) const
{
    if (HueNodeFactoryPrivate::bCreatingNativeSpecializedWidget)
    {
        return nullptr;
    }

    if (!FHueStyleResolver::CanStyleNode(Node))
    {
        return nullptr;
    }

    UK2Node* K2Node = Cast<UK2Node>(Node);
    if (!K2Node)
    {
        return nullptr;
    }

    // Reaching Hue's registered node factory is itself the proof that the
    // node did not pre-empt factories with CreateVisualWidget(). Mark support
    // before construction so Hue's pin factory can tint exec pins while the
    // Slate node is being built.
    FHueStyleResolver::MarkVisualSupport(K2Node);

    // Keep this ordering aligned with Unreal's stock FNodeFactory so a broad
    // interface such as Add Pin never steals a more specialized presentation.
    if (UK2Node_Composite* CompositeNode = Cast<UK2Node_Composite>(K2Node))
    {
        return SNew(SHueGraphNodeK2Composite, CompositeNode);
    }

    if (K2Node->DrawNodeAsVariable())
    {
        return SNew(SHueGraphNodeK2Var, K2Node);
    }

    if (UK2Node_Switch* SwitchNode = Cast<UK2Node_Switch>(K2Node))
    {
        return SNew(SHueGraphNodeK2Switch, SwitchNode);
    }

    if (UK2Node_PromotableOperator* OperatorNode =
        Cast<UK2Node_PromotableOperator>(K2Node))
    {
        return SNew(SHueGraphNodeK2Operator, OperatorNode);
    }

    if (K2Node->GetClass()->ImplementsInterface(
        UK2Node_AddPinInterface::StaticClass()))
    {
        return SNew(SHueGraphNodeK2Sequence, K2Node);
    }

    if (UK2Node_Timeline* TimelineNode = Cast<UK2Node_Timeline>(K2Node))
    {
        return SNew(SHueGraphNodeK2Timeline, TimelineNode);
    }

    // These node families rely on GraphEditor-private Slate classes. Ask
    // Unreal to construct its own specialized widget, then bind Hue only to
    // standard visual layers. Native controls and interaction remain intact.
    if (HueNodeFactoryPrivate::UsesNativeSpecializedBridge(K2Node))
    {
        return HueNodeFactoryPrivate::CreateNativeSpecializedWidget(K2Node);
    }

    if (UK2Node_Event* EventNode = Cast<UK2Node_Event>(K2Node))
    {
        return SNew(SHueGraphNodeK2Event, EventNode);
    }

    if (UK2Node_FormatText* FormatTextNode = Cast<UK2Node_FormatText>(K2Node))
    {
        return SNew(SHueGraphNodeFormatText, FormatTextNode);
    }

    // Generic fallback includes compact K2 nodes. SHueGraphNodeCallFunction
    // preserves Unreal's default compact layout and binds Hue to the compact
    // fill/title after native construction.
    return SNew(SHueGraphNodeCallFunction, K2Node);
}
