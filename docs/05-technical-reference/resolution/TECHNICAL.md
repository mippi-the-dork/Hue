# Style Identity and Color Resolution — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Typed global keys

FHueStyleResolver::GetGlobalKey constructs keys for supported K2 node families. It deliberately retains preexisting raw UFunction paths for Global Function rules created by earlier builds.

## Channel resolution

ResolveColor(Node,Channel,OutColor) checks explicit scoped styles in precedence order, independently for each EHueStyleChannel. GetEffectiveCategoryColor handles nested paths and reports the matching source.

## Native fallback

GetNativeColor supplies Unreal's normal appearance if no Hue channel override applies. GetPickerInitialColor can display an inherited/effective value even when the current scope has no explicit override.

## Panel diagnostics

GetSingleSelectionSourceText and GetInheritedSourceTooltip explain the origin of a selected channel, including named parent Category keys and the applicable Global label.

## Avoiding expensive work

Member-category migration happens on Blueprint change/compile callbacks. Normal graph painting should not rebuild the complete category hierarchy for each color read.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)
- [HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h)

For user-facing behavior, [read Design](DESIGN.md). Source inspection alone does not establish completed live Unreal Editor testing.
