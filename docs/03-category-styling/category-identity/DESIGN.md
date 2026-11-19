# My Blueprint Category Styles — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## What Category means

<!-- Media target: IMAGE | category-member-link | docs/assets/media/category-member-link.png -->
<figure class="doc-media" data-kind="image" data-media-id="category-member-link">
  <img src="../../assets/media-placeholder.png" alt="Hue's Category scope follows a member's actual My Blueprint Category." loading="lazy">
  <figcaption>Hue's Category scope follows a member's actual My Blueprint Category.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Show a Blueprint's My Blueprint panel with a custom function assigned to Combat, its call node selected in the graph and the Hue Category section active.</span></figcaption>
</figure>


Hue Category styling follows user-defined **My Blueprint** member categories—not action-menu groupings, native C++ node classes or coincidentally matching node titles. It applies to supported functions, variables, macros and events with an applicable defining Blueprint member category.

## Style a member category

1. Assign the member to a category in My Blueprint.
2. Place a compatible call or reference node in a graph.
3. Select it; open the Hue panel and expand Category.
4. Choose a Header Color, Body Color or text channel.
5. Inspect other eligible nodes associated with the category; they can inherit the style.

## Common problems

Category may be unavailable for generic operators, built-in nodes or unsupported special presentations. Choose Instance or Global when a My Blueprint member category does not exist.

Category rules save with their **defining Blueprint**, so save and submit that asset after styling.

---

For native implementation detail, [read Technical](TECHNICAL.md).
