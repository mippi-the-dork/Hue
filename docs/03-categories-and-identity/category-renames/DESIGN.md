# Category Rename Persistence — Design

**Hue v1.0.0 · User workflow**

## Renaming a Category should not erase its style

Hue watches eligible My Blueprint Category changes and migrates applicable Category style keys when a Category is renamed. Nested styled Category paths can follow an eligible parent rename.

<figure class="doc-media" data-kind="gif" data-media-id="category-rename-preservation"><img src="../../assets/media-placeholder.png" alt="Hue Category rename preserving inherited node colors" loading="lazy"><figcaption>A Hue Category style remains associated with its Blueprint member hierarchy after a Category rename.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a Blueprint with styled My Blueprint Categories Attack and Attack|Weapons. Rename the parent Attack to Combat through the native Blueprint Category editing workflow. Show the updated Combat and Combat|Weapons paths and confirm their previously styled nodes retain their Hue colors.</span></figcaption></figure>

## Example

If `Attack` and `Attack|Weapons` carry explicit Hue rules, renaming the parent to `Combat` can migrate those keys to `Combat` and `Combat|Weapons` as part of Hue's recognized rename workflow.

## Moving a member is different

Moving **one function** from `Combat` to `Utility` is not necessarily a Category rename. Hue does not move all `Combat` styling to `Utility` just because a member was reassigned. The existing Category rule remains a rule for the original Category.

## What to verify

After reorganizing a Blueprint's Categories, inspect the Hue panel's source labels on a representative member, then save the defining Blueprint. If a style appears to have changed unexpectedly, check whether the action was a genuine Category rename, a move of an individual member, or a change in which parent Category is closest.

## Team behavior

Category style rules belong to the defining Blueprint asset. Their migration is part of the Blueprint editing/save workflow and should be reviewed like other Blueprint asset changes in Perforce or Git.

---

For implementation and storage, [read Technical](TECHNICAL.md).
