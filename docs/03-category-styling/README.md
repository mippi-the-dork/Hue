# 03. Category Styling

**Hue v1.0.0 · My Blueprint categories, nested inheritance and rename migration**

Category is Hue's most useful way to organize a **large family of Blueprint members** without individually painting every node. It follows user-authored My Blueprint categories, not the generic action-menu classification.

## Example category layout

<!-- Media target: IMAGE | category-inheritance-tree | docs/assets/media/category-inheritance-tree.png -->
<figure class="doc-media" data-kind="image" data-media-id="category-inheritance-tree">
  <img src="../assets/media-placeholder.png" alt="Nested My Blueprint categories let related members share parent styles without copying values." loading="lazy">
  <figcaption>Nested My Blueprint categories let related members share parent styles without copying values.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Show My Blueprint Combat, Combat|Weapons and Combat|Weapons|Melee with matching graph nodes; label parent and inherited colors.</span></figcaption>
</figure>


A team might organize members under Combat, Combat|Weapons and Combat|Weapons|Melee. Set a Header Color on Combat, override only the Header Color on Weapons, and let Melee inherit the nearest applicable value. Body Color can continue coming from Combat.

## Rename versus move

Renaming an actual category can migrate its Hue style rules. Moving an individual member to another category instead changes which category style that member can inherit; it does not rename the old shared category.

## What to verify

Check the defining Blueprint, source labels and any nested category members after significant reorganizations. Save the defining Blueprint so its CategoryStyles persist.

## Topics in this section

| Topic | What you will learn | Design | Technical |
| --- | --- | --- | --- |
| **My Blueprint Category Styles** | Connect styles to the actual defining Blueprint member categories | [Design](category-identity/DESIGN.md) | [Technical](category-identity/TECHNICAL.md) |
| **Nested Category Inheritance** | Use parent categories as reusable per-channel style sources | [Design](nested-inheritance/DESIGN.md) | [Technical](nested-inheritance/TECHNICAL.md) |
| **Renaming Categories and Moving Members** | Preserve styled categories while distinguishing moves from renames | [Design](renames-and-moves/DESIGN.md) | [Technical](renames-and-moves/TECHNICAL.md) |

Read **Design** for user behavior and **Technical** for the source implementation. The guides complement one another rather than duplicating the same walkthrough.
