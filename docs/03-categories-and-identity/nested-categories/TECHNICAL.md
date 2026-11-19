# Nested My Blueprint Category Inheritance — Technical

**Hue v1.0.0 · C++ implementation**

## Category path model

`FHueBlueprintCategoryInfo` identifies the defining Blueprint, relevant member graph and Category string. The resolver uses this source information instead of assuming all displayed nodes belong to a Category in their current graph.

## Lookup order

`GetEffectiveCategoryColor` tests exact Category first, then nearest parent, then higher parents, using the `|` hierarchy delimiter. It searches **per `EHueStyleChannel`** and returns the source Category that supplied the matching override.

## Storage

`UHueBlueprintExtension::CategoryStyles` stores only explicitly styled Category paths. Derived descendants do not need serialized copies of inherited colors. This design allows changing a parent style to affect all currently inheriting descendants without rewriting every child entry.

## Rename relation

Rename migration is a separate event-driven procedure, not a special behavior of the normal color resolver. See [Category Rename Migration](../../03-categories-and-identity/category-renames/TECHNICAL.md).

[HueStyleResolver.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.h) · [HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h).

---

For user-facing workflows and examples, [read Design](DESIGN.md).
