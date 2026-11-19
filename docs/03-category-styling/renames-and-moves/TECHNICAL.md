# Renaming Categories and Moving Members — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Rename migration hooks

FHueStyleResolver::PrimeCategoryTracking records the initial category/member snapshot. NotifyBlueprintChanged responds to Blueprint changes or compilation, calling RefreshCategoryTrackingAndMigrate.

## Migration helpers

DetectCategorySortRenames, TryApplyCategoryRenamePairs and MigrateCategoryStyle compare category paths and members to rekey applicable Blueprint-owned CategoryStyles.

## Performance and safety

Migration is triggered by Blueprint changes, not by scans on every graph paint. It is a conservative heuristic for supported rename workflows, not a guarantee for every unusual refactor. Validation should cover parent/child renames, single-member moves, Undo and reload.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueMenu.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueMenu.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)

For editor usage, [read Design](DESIGN.md). Source-reviewed claims are not equivalent to completed UE editor QA.
