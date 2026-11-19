# Category Rename Persistence — Technical

**Hue v1.0.0 · Unreal implementation reference**

## Event-driven rename migration

`FHueModule` subscribes to Blueprint change/compile callbacks and calls `FHueStyleResolver::NotifyBlueprintChanged`. `PrimeCategoryTracking` initializes tracked category structure on editor opening.

`HueStyleResolverPrivate::RefreshCategoryTrackingAndMigrate` compares recorded user-defined member Category state, identifies eligible rename pairs and applies appropriate updates to `CategoryStyles`. Nested style paths can be migrated when a parent path is renamed.

## Not a naive text replacement

A single member moving from one category to another is not sufficient to rewrite a shared style key. The migration code distinguishes category renames from ordinary member reorganization and handles conflicting/previously existing style keys conservatively.

## Performance

Rename detection runs in response to editor structure changes, not inside every `ResolveColor` or Slate widget paint. This prevents category discovery from becoming a recurring graph-rendering cost.

## Persistence owner

Changed Category style keys live in `UHueBlueprintExtension` attached to the relevant Blueprint. The usual Blueprint package save persists that migration.

[HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp) · [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp).

---

For usage and examples, [read Design](DESIGN.md).
