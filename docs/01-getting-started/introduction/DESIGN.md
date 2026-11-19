# What Hue Does — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## The problem Hue solves

<!-- Media target: IMAGE | introduction-color-language | docs/assets/media/introduction-color-language.png -->
<figure class="doc-media" data-kind="image" data-media-id="introduction-color-language">
  <img src="../../assets/media-placeholder.png" alt="A consistent palette can make movement, combat and UI logic easier to recognize at a glance." loading="lazy">
  <figcaption>A consistent palette can make movement, combat and UI logic easier to recognize at a glance.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture a nontrivial but readable Blueprint with movement nodes blue, combat nodes red and UI helpers purple; leave pin connections and labels visible.</span></figcaption>
</figure>


A Blueprint can be logically correct yet difficult to read. Hue adds a visual layer to compatible K2 Blueprint nodes, helping communicate which nodes belong to movement, combat, queries, UI, or other gameplay concerns. It styles the editor presentation without changing node execution, data flow, compilation, or packaged-game behavior.

## What you can style

Hue offers four independent controls: **Header Color**, **Header Text Color**, **Body Color**, and **Body Text Color**. You can alter a single channel without repainting the rest of the node.

| Need | Choose |
| --- | --- |
| Highlight an exceptional call in this graph | **Instance** styling |
| Use a shared style for members in a My Blueprint category | **Category** styling |
| Recognize the same logical function/variable/event across Blueprints | **Global** styling |

## Example: a readable gameplay graph

You might give movement nodes blue headers, combat-related members red headers, and UI helpers purple headers. An individual critical node could override only its Body Text Color to draw attention. Clearing that override reveals the inherited color underneath.

## Boundaries

Hue works only on **supported Blueprint node presentations**. Reroute and Documentation nodes are intentionally unsupported in v1.0.0. Nodes with nonstandard native interfaces may remain untouched; that protects their editing controls.

## Start here

[Install Hue](../installation/DESIGN.md) is not a valid separate topic; instead follow [Installation and Compatibility](../installation/DESIGN.md) in this section, then [Your First Styled Node](../first-use/DESIGN.md).

---

For the underlying C++ implementation, see [Technical](TECHNICAL.md).
