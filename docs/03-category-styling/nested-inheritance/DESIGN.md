# Nested Category Inheritance — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Inheritance follows the hierarchy

<!-- Media target: IMAGE | nested-category-graphic | docs/assets/media/nested-category-graphic.png -->
<figure class="doc-media" data-kind="image" data-media-id="nested-category-graphic">
  <img src="../../assets/media-placeholder.png" alt="A child Category inherits the closest parent style for each individual channel." loading="lazy">
  <figcaption>A child Category inherits the closest parent style for each individual channel.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Show Combat and Combat|Weapons/Combat|Weapons|Melee member nodes with Header Color inherited from the nearest parent and Body Color inherited from the higher parent.</span></figcaption>
</figure>


Unreal uses the vertical bar in My Blueprint category paths: Combat, Combat|Weapons, Combat|Weapons|Melee. A style applied to Combat can be inherited by either child when a closer override is absent.

## Nearest parent, one channel at a time

If Combat defines a red header and gray body, while Combat|Weapons defines an orange header only, a Melee node can have **orange Header Color from Weapons** and **gray Body Color from Combat**.

## Try it

<!-- Media target: GIF | nested-category-override | docs/assets/media/nested-category-override.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="nested-category-override">
  <img src="../../assets/media-placeholder.png" alt="Closer Category overrides supersede a parent only for the channels they define." loading="lazy">
  <figcaption>Closer Category overrides supersede a parent only for the channels they define.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record styling Combat, then Combat|Weapons Header Color, then clearing the Weapons override; keep the My Blueprint hierarchy and relevant nodes in view.</span></figcaption>
</figure>


1. Style a parent category's Header Color.
2. Confirm a child member inherits it.
3. Override the Header Color on the nearer child category.
4. Clear that child override; the original parent's header should immediately return.

## What can surprise you

Editing a parent changes every eligible descendant without a closer override for that particular channel. Preview the current source label and the affected hierarchy before changing a heavily reused parent category.

---

For native implementation detail, [read Technical](TECHNICAL.md).
