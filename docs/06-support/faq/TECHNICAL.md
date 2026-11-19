# Frequently Asked Questions — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## FAQ diagnostic ownership

- Missing tab/menu: HueModule Blueprint tab and ToolMenus integration
- Unavailable Category: GetBlueprintCategoryInfo
- Unexpected Global grouping: GetGlobalKey
- Mixed color/source: SHuePanel override and swatch state functions
- Unexpected inheritance: ResolveColor and GetEffectiveCategoryColor
- Node unchanged: CanStyleNode versus IsSupportedNode, plus decorator/factory safety path
- Save discrepancy: HueBlueprintExtension or HueSettings ownership

## Reproduction strategy

Start with a standard Call Function in a disposable Blueprint, then test Category and Global scope separately. For native presentation problems, capture the node class and Slate-specific behavior. For style persistence, compare before/after Blueprint asset and project Hue config state.

## Verification distinction

These source-grounded explanations do not certify every custom node type, every rename edge case or every UE minor version without actual editor testing.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)
- [HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h)

For user-facing behavior, [read Design](DESIGN.md). Source inspection alone does not establish completed live Unreal Editor testing.
