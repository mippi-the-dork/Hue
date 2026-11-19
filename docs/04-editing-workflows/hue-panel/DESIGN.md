# Hue Panel — Design

**Hue v1.0.0 · User workflow**

## Your primary styling workspace

<!-- Media target: IMAGE | hue-panel-anatomy | docs/assets/media/hue-panel-anatomy.png -->
<figure class="doc-media" data-kind="image" data-media-id="hue-panel-anatomy">
  <img src="../../assets/media-placeholder.png" alt="The Hue panel exposes channel swatches, available scopes, clearing tools and source reporting." loading="lazy">
  <figcaption>The Hue panel exposes channel swatches, available scopes, clearing tools and source reporting.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture the full docked Hue panel beside a selected compatible function node, showing all four channel rows and Instance/Category/Global scopes.</span></figcaption>
</figure>


The Hue tab appears inside a supported Blueprint Editor. It observes graph selection and presents **Instance**, **Category** and **Global** scope sections where applicable. Each scope contains independent rows for Header Color, Header Text Color, Body Color and Body Text Color.


## What the labels mean

**Override** means the current scope explicitly owns the channel. A Category or Parent label identifies the inherited rule. A Global label names the logical identity type. **Unreal Default** means no Hue override exists for that channel. **Multiple Values** means the selected scope targets do not share a uniform state or ownership.

## A clear editing sequence

<!-- Media target: GIF | picker-editing-demo | docs/assets/media/picker-editing-demo.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="picker-editing-demo">
  <img src="../../assets/media-placeholder.png" alt="The native color picker updates the selected node's supported visual regions." loading="lazy">
  <figcaption>The native color picker updates the selected node's supported visual regions.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record opening a Header Color picker in Hue, adjusting to a new color, applying, and using Unreal Undo; keep the selected node visible.</span></figcaption>
</figure>


1. Select one compatible graph node.
2. Expand the intended scope, not necessarily Instance.
3. Choose the exact channel you need and use the native color picker.
4. Confirm the node's visible result and effective source.
5. Use **Clear** for a single channel, or **Clear All Overrides** for the scope.
6. Undo if the change was unintended.

## Why the panel might not show a scope

A node that lacks an applicable My Blueprint Category cannot be Category-styled. A node without a recognized Global identity cannot be Global-styled. Unsupported selected nodes are not modified even when other selected nodes are valid.

## Multiple selection

The panel counts compatible targets and unique Category/Global identities. It may report Multiple Values where targets look identical but have different override ownership. Check target counts before applying a batch style.

---

For implementation and storage, [read Technical](TECHNICAL.md).
