# Style Precedence and Effective Sources — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## How one color is chosen

<!-- Media target: IMAGE | precedence-stacked | docs/assets/media/precedence-stacked.png -->
<figure class="doc-media" data-kind="image" data-media-id="precedence-stacked">
  <img src="../../assets/media-placeholder.png" alt="A node's channels can resolve from different scopes at the same time." loading="lazy">
  <figcaption>A node's channels can resolve from different scopes at the same time.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture the Hue panel with Header Color sourced from Category, Body Color from Global and Body Text Color from Instance; show the styled node beside it.</span></figcaption>
</figure>


For each of the four channels, Hue looks for the first applicable rule in this order:

1. **Instance**
2. **Exact Category**
3. **Nearest styled parent Category**, then more distant parents
4. **Global**
5. **Unreal Default**

The order repeats independently for Header Color, Header Text Color, Body Color, and Body Text Color.

## Example: three scopes on one node

Suppose Global supplies a blue Header and dark gray Body. Combat Category supplies a red Header. A specific node's Instance rule supplies yellow Body Text. The result is red Header, inherited native Header Text, dark gray Body, and yellow Body Text.

## Read the current source

<!-- Media target: GIF | precedence-clearing-demo | docs/assets/media/precedence-clearing-demo.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="precedence-clearing-demo">
  <img src="../../assets/media-placeholder.png" alt="Changing a more specific rule instantly reveals the lower-precedence color underneath." loading="lazy">
  <figcaption>Changing a more specific rule instantly reveals the lower-precedence color underneath.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record one node with a Global Header, then a Category Header, then an Instance Header; clear each in reverse order with the Hue panel source label visible.</span></figcaption>
</figure>


The Hue panel can report **Override**, **Category: Combat|Weapons**, **Parent: Combat**, **Global Function/Variable/Event/etc.**, or **Unreal Default**. Those labels tell you *who owns the effective value*, not just what color is visible.

## Clearing reveals the next source

Clearing the Instance Body Text override doesn't force a default if Category or Global still owns that channel. It reveals the next applicable rule. Clearing the Category Header then reveals Global's blue Header.

## Why two same-looking nodes can differ

One may explicitly override a color while the other inherits that same color. In batch editing, Hue may correctly show **Multiple Values** because the underlying sources differ.

---

For the underlying C++ implementation, see [Technical](TECHNICAL.md).
