# Renaming Categories and Moving Members — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Rename a styled category

<!-- Media target: GIF | category-rename | docs/assets/media/category-rename.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="category-rename">
  <img src="../../assets/media-placeholder.png" alt="Renaming an actual Category can preserve its associated Hue styling throughout its hierarchy." loading="lazy">
  <figcaption>Renaming an actual Category can preserve its associated Hue styling throughout its hierarchy.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record styled parent and child My Blueprint categories, rename the parent, and show both graph nodes retaining colors and updated source labels.</span></figcaption>
</figure>


When a user renames a My Blueprint category, Hue tracks the Blueprint structural change and migrates applicable saved category styling. Nested styled children can move with the renamed parent.

## Moving a member is different

Moving one function from Combat to Utility does **not** rename Combat. The original Combat rule remains; that member can instead pick up Utility's style.

## Confirm a rename

<!-- Media target: GIF | member-move-demo | docs/assets/media/member-move-demo.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="member-move-demo">
  <img src="../../assets/media-placeholder.png" alt="Moving one member to a new Category differs from renaming the Category itself." loading="lazy">
  <figcaption>Moving one member to a new Category differs from renaming the Category itself.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a styled Combat category with two members; move only one member to Utility and show Combat keeps its original style while the moved member follows its new category.</span></figcaption>
</figure>


In a test Blueprint, style a parent category and a child, rename the parent through My Blueprint, and inspect their effective colors and source labels. Save and reopen to check persistence.

## A caveat for large reorganizations

Rename detection is event-driven and must distinguish genuine renames from unrelated membership changes. For complex restructures, verify the outcome and use Unreal Undo if needed. Don't assume a whole collection was rekeyed simply because one member moved.

---

For native implementation detail, [read Technical](TECHNICAL.md).
