# First Use — Design

**Hue v1.0.0 · User guide**

## Your first style in five steps

<!-- Media target: GIF | first-use-color | docs/assets/media/first-use-color.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="first-use-color">
  <img src="../../assets/media-placeholder.png" alt="A single Instance override changes only the selected node, then clears back to its inherited appearance." loading="lazy">
  <figcaption>A single Instance override changes only the selected node, then clears back to its inherited appearance.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record selecting a Call Function node, opening the Hue panel, setting Instance Header Color in the native picker, then clearing it with another call to the same function visible.</span></figcaption>
</figure>


1. Open an ordinary Blueprint with a function or variable node.
2. If necessary, restore the **Hue** panel from the Blueprint Editor's tab menu.
3. Select one compatible node on the graph. Hue updates its panel for that selection.
4. Expand **Instance**, choose **Header Color**, and pick a clearly different color. The selected node's title region changes.
5. Use **Clear** on Header Color. The node returns to its inherited Category/Global color or the native Unreal default.


## Understand the four rows

Every applicable scope can specify **Header Color**, **Header Text Color**, **Body Color**, and **Body Text Color**. Some compact node families share a single fill rather than separate header/body surfaces. Hue preserves native controls such as numeric fields and dropdowns.

## Understand the three scopes

Instance affects one node. Category uses the node's **My Blueprint** member Category, not the Blueprint action menu Category. Global uses a logical node identity determined by its type. A scope may be unavailable if no corresponding identity exists.

## Try your first inherited style

Give an applicable Category a Header Color, then select one of its member nodes and set a different Instance Body Color. The node can display both. Clear the Instance Body Color and watch the Category/Global/default source reappear for that channel.

## Before editing many nodes

When multiple nodes are selected, check the supported selection count, unique Category/Global target count and Multiple Values labels. Batch changes occur in one Undo transaction; test in a disposable Blueprint before applying a new palette across production content.

---

For Unreal implementation, data ownership and boundaries, [read Technical](TECHNICAL.md).
