# Multi-Selection and Mixed Values — Design

**Hue v1.0.0 · User guide**

## Edit several nodes with one operation

Hue supports batch edits across nonadjacent compatible nodes, even when they are different function calls, variables, Categories or node classes. You can edit Instance colors for each node or deduplicate shared Category/Global targets.

<figure class="doc-media" data-kind="gif" data-media-id="hue-batch-undo"><img src="../../assets/media-placeholder.png" alt="Hue multi-selection edit and one-step Undo demonstration" loading="lazy"><figcaption>Hue applies a batch color change in one transaction, allowing the entire compatible selection to be undone at once.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record selecting at least five visibly different supported node types and one unsupported reroute. Show the Hue panel's compatible/unsupported counts and Multiple Values, set Instance Body Color across the selection, then press Undo once to restore the prior colors. Keep the panel and graph visible.</span></figcaption></figure>

## What each scope edits

- **Instance:** Every selected compatible node instance, so five nodes generally mean five targets.
- **Category:** The *unique* applicable My Blueprint Categories represented by the selection. Multiple nodes in `Combat` share one Category target.
- **Global:** Unique logical identities. Repeated calls to the same function are one Global Function target; Get and Set of the same variable share one Global Variable identity.

## Understanding Multiple Values

The panel can report **Multiple Values** when explicit colors differ, inherited colors differ, or override ownership differs—even if some nodes appear the same color. Setting a new color across eligible targets produces a uniform explicit value for that channel at the chosen scope.

## Mixed supported and unsupported nodes

Unsupported nodes can remain selected. Hue leaves them unchanged and reports them separately; the compatible subset can still be edited.

## Safe practice

Before applying a broad Category or Global edit, check the unique target count and whether the selection crosses systems. Afterward, inspect a few representative nodes, especially those that previously inherited a parent rule.

---

For the Unreal implementation, [read Technical](TECHNICAL.md).
