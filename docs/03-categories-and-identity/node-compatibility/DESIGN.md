# Supported Nodes and Native Controls — Design

**Hue v1.0.0 · User workflow**

## Supported does not mean every graph node

Hue styles compatible **K2 Blueprint node presentations**, including ordinary function calls, variables, events and multiple specialized families. It avoids taking over controls that need Unreal's native behavior.

<figure class="doc-media" data-kind="image" data-media-id="node-family-comparison"><img src="../../assets/media-placeholder.png" alt="Hue styling across several supported K2 node types" loading="lazy"><figcaption>Hue applies its visual channels to different supported Blueprint node families while leaving their native interaction controls intact.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture a graph containing a function-call node, variable Get/Set, event, branch/switch, compact operator and sequence/timeline where compatible. Apply distinct Hue colors but leave pins and value-entry controls visible and readable. Include at least one unsupported reroute node for comparison.</span></figcaption></figure>

## Representative supported families

Normal function calls, variables, events, compact operators and supported native-specialized node presentations such as composite, switch, timeline, sequence and format-text families are handled through appropriate UI paths. Actual styling opportunities depend on whether the live node presents compatible visual surfaces.

## Intentionally unsupported

- **Reroute nodes** lack meaningful Header/Body surfaces matching Hue's four channels.
- **Documentation nodes** are intentionally not styled in v1.0.0.
- **Unrecognized custom node presentations** remain native if Hue cannot identify safe surfaces.

## What Hue intentionally does not recolor

Editable numeric fields, dropdowns, checkboxes, class pickers and object pickers remain Unreal-owned. Body Text Color may tint supported pin labels and execution triangles, but must not replace or interfere with native value-entry controls.

## When a selected node is unsupported

Leave it selected if you want; Hue reports unsupported targets and avoids applying changes to them. A mixed batch can still edit supported nodes. If an expected node family does not style, record its exact C++ node type, native widget presentation and the Hue/UE version before reporting a bug.

---

For implementation and storage, [read Technical](TECHNICAL.md).
