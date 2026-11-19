# Four Visual Channels — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Style representation

`FHueNodeStyleOverride` owns independent `bOverrideHeaderColor`, `bOverrideHeaderTextColor`, `bOverrideBodyColor` and `bOverrideBodyTextColor` flags, each paired with an `FLinearColor`.

## Effective color selection

`FHueStyleResolver::ResolveColor(Node, Channel, OutColor)` runs the precedence chain independently for the requested `EHueStyleChannel`. `GetNativeColor` and `GetPickerInitialColor` supply appropriate fallbacks when the current scope has no override.

## Graph node rendering

Hue's custom `SHueGraphNode*` widgets bind colors into selected header/body/title elements. For nodes with specialized native Slate presentation, `FHueNativeNodeDecorator` attaches to compatible existing surfaces rather than replacing the whole widget.

## Important boundaries

Native editable Slate controls are not universally recolored by Body Text Color. Disabled or unrelated node states retain Unreal's native subdued-opacity cues; plugin styles must not imply that a disabled node is active.

---

## Implementation references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)

For practical user workflows, see [Design](DESIGN.md). Source-reviewed behavior is distinguished from unexecuted Unreal Editor QA.
