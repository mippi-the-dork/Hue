# My Blueprint Category Styles — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Category identity and storage

<!-- Media target: IMAGE | category-ownership-tech | docs/assets/media/category-ownership-tech.png -->
<figure class="doc-media" data-kind="image" data-media-id="category-ownership-tech">
  <img src="../../assets/media-placeholder.png" alt="Category styles are stored on the Blueprint defining the relevant member." loading="lazy">
  <figcaption>Category styles are stored on the Blueprint defining the relevant member.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Diagram a function call in one Blueprint referencing a member defined in another, and show CategoryStyles being read from the defining Blueprint.</span></figcaption>
</figure>


The resolver's GetBlueprintCategoryInfo returns the defining Blueprint, member graph and user-defined category path. HueBlueprintExtension.h defines CategoryStyles as a map of category strings to four-channel style overrides.

## Applicability

The Hue panel and context menu validate the category identity before offering it. No category can be fabricated from a node title or action-menu section.

## Lookup

GetEffectiveCategoryColor checks explicit and ancestral category keys on the defining Blueprint. Rules are stored at the category level rather than copied onto each individual node.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueMenu.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueMenu.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)

For editor usage, [read Design](DESIGN.md). Source-reviewed claims are not equivalent to completed UE editor QA.
