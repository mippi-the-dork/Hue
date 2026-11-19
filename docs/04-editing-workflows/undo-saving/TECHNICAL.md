# Undo, Persistence and Teams — Technical

**Hue v1.0.0 · Source-level implementation**

## Blueprint extension

`UHueBlueprintExtension` is a `UBlueprintExtension` storing `InstanceStyles` and `CategoryStyles`. `FHueStyleResolver::GetOrCreateBlueprintExtension` ensures style data is associated with the relevant Blueprint and can participate in normal UObject transaction/package serialization.

## Project config

`UHueSettings` derives from `UDeveloperSettings`, has `UCLASS(Config=Editor,DefaultConfig)`, and persists `GlobalFunctionStyles` using config. The map name survives from earlier function-only styles and remains the data owner for typed Global keys.

## One transaction per batch

Resolver setters use `FScopedTransaction`, `Modify`, appropriate Blueprint edit/change notifications and Slate invalidation. Config changes invoke `SaveConfig`. These are editor transactions and settings writes, not runtime asset changes.

## Team integration boundary

A style change affects its owner: the Blueprint package or the shared editor config. Hue does not directly implement source-control checkout, resolve conflicts or guarantee a file is writable. The plugin installation directory is separate from stored styles.

[HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h) · [HueSettings.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueSettings.cpp) · [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp).

---

For normal usage and examples, [read Design](DESIGN.md).
