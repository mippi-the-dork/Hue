# What Hue Does — Design

**Hue v1.0.0 · User guide**

## The problem Hue solves

When a Blueprint grows, nodes from unrelated systems can become hard to distinguish. Hue lets you create a consistent visual vocabulary—for example, green for pure queries, blue for movement, and red for operations that change critical state. It adds an extra layer of meaning to **compatible K2 node presentations**, without rewriting any Blueprint behavior.

<figure class="doc-media" data-kind="image" data-media-id="overview-workflow"><img src="../../assets/media-placeholder.png" alt="Unreal Blueprint graph with several Hue-styled node groups" loading="lazy"><figcaption>A styled Blueprint graph can distinguish related systems at a glance while preserving native execution and data connections.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Show a real Blueprint graph with at least three groups of nodes using different Hue colors for identifiable gameplay functions, with wires, titles and the Hue panel visible. Use meaningful colors rather than a random rainbow.</span></figcaption></figure>

## What you can style

Hue offers four channels on supported nodes: **Header Color**, **Header Text Color**, **Body Color**, and **Body Text Color**. They are independent. You can override just a header without losing a body color inherited from another scope.

## Three layers of meaning

- **Instance:** One specific node, ideal for an exceptional use.
- **Category:** Blueprint members organized in a user-defined **My Blueprint Category**, including nested parent Categories.
- **Global:** The same logical node identity throughout the project, such as a function, variable, event, macro or supported operator.

## What this means in practice

Choose a visual language, apply broad shared rules first, then add narrower exceptions only where useful. Clear an exception to reveal the next inherited color. A node's functionality and compiled behavior do not change.

## Where Hue deliberately stops

Hue is editor-only. It does not recolor a packaged game, change Blueprint logic or recolor every native input widget. Reroute and Documentation nodes are intentionally not supported in v1.0.0. A specialized custom node may remain unchanged if Hue cannot confirm a safe visual path.

---

For Unreal implementation, data ownership and boundaries, [read Technical](TECHNICAL.md).
