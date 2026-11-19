# Multi-Selection and Mixed Values — Design

**Hue v1.0.0 · User guide**

## Edit several nodes with one operation

<!-- Media target: IMAGE | multi-selection-view | docs/assets/media/multi-selection-view.png -->
<figure class="doc-media" data-kind="image" data-media-id="multi-selection-view">
  <img src="../../assets/media-placeholder.png" alt="Hue reports mixed values and eligible target counts across a Blueprint graph selection." loading="lazy">
  <figcaption>Hue reports mixed values and eligible target counts across a Blueprint graph selection.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Select at least four supported node types, with different current colors, and show the Hue panel displaying Multiple Values and target counts.</span></figcaption>
</figure>


Hue supports batch edits across nonadjacent compatible nodes, even when they are different function calls, variables, Categories or node classes. You can edit Instance colors for each node or deduplicate shared Category/Global targets.


## What each scope edits

- **Instance:** Every selected compatible node instance, so five nodes generally mean five targets.
- **Category:** The *unique* applicable My Blueprint Categories represented by the selection. Multiple nodes in `Combat` share one Category target.
- **Global:** Unique logical identities. Repeated calls to the same function are one Global Function target; Get and Set of the same variable share one Global Variable identity.

## Understanding Multiple Values

<!-- Media target: GIF | batch-undo-demo | docs/assets/media/batch-undo-demo.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="batch-undo-demo">
  <img src="../../assets/media-placeholder.png" alt="One Undo reverses an entire Hue batch edit on the eligible selection." loading="lazy">
  <figcaption>One Undo reverses an entire Hue batch edit on the eligible selection.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record several differently colored supported nodes selected together, set Instance Body Color once, then Undo once and show all previous colors return.</span></figcaption>
</figure>


The panel can report **Multiple Values** when explicit colors differ, inherited colors differ, or override ownership differs—even if some nodes appear the same color. Setting a new color across eligible targets produces a uniform explicit value for that channel at the chosen scope.

## Mixed supported and unsupported nodes

Unsupported nodes can remain selected. Hue leaves them unchanged and reports them separately; the compatible subset can still be edited.

## Safe practice

Before applying a broad Category or Global edit, check the unique target count and whether the selection crosses systems. Afterward, inspect a few representative nodes, especially those that previously inherited a parent rule.

---

For the Unreal implementation, [read Technical](TECHNICAL.md).
