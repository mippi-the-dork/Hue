# Editor Integration Architecture — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## How Hue fits into Unreal

<!-- Media target: IMAGE | native-presentation-bridge | docs/assets/media/native-presentation-bridge.png -->
<figure class="doc-media" data-kind="image" data-media-id="native-presentation-bridge">
  <img src="../../assets/media-placeholder.png" alt="Hue uses the correct visual integration path for each supported Blueprint node family." loading="lazy">
  <figcaption>Hue uses the correct visual integration path for each supported Blueprint node family.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Draw two paths: Hue custom SGraphNode wrapper for an ordinary node, and native widget decoration for specialized presentations, both fed by one style resolver.</span></figcaption>
</figure>


Hue is an **Editor-only Blueprint visualization plugin**. It adds a dockable Hue panel, context-menu commands and compatible node rendering while keeping Unreal's Blueprint compiler, graph connections and native controls intact.

## What it doesn't install

You don't need to place a Hue Actor, create a component or add a runtime subsystem. Styling lives with Blueprint or project editor data, not in your game's execution flow.

## Why native rendering matters

Some node families have their own specialized interfaces. Hue sometimes creates a compatible graph node widget and sometimes decorates Unreal's existing one. When neither is safe, the node remains unchanged.

## What users can expect

A style should appear again when the Blueprint is reopened, provided its applicable data has been saved. Global styles appear wherever the same supported logical identity resolves and the project config is shared.

## Boundaries

Editor module integrations depend on native UE 5.8 graph and Slate APIs. The plugin does not promise support for every third-party custom Blueprint node.

---

For native implementation details, [read Technical](TECHNICAL.md).
