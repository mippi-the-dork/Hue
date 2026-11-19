# Clearing Overrides and Undo — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## How clearing is implemented

`FHueStyleResolver::ClearColorOverride` and `ClearAllOverrides` delegate to batch operations. A channel's override-enabled flag is cleared; unused empty style records can be removed by internal cleanup.

## Batch transaction model

`SetColorOverrides`, `ClearColorOverrides`, and `ClearAllOverrides` collect **unique scope representatives** and perform one transaction, one relevant config save, and one Slate invalidation for the batch rather than duplicating writes for repeated identities.

## Persistence

Instance/Category operations modify the applicable `UBlueprint` extension and trigger Blueprint save/dirty behavior. Global actions operate on `UHueSettings` editor configuration and use `SaveConfig`.

## Color-picker behavior

`SHuePanel::OnSetColorClicked` launches native Slate color selection with a starting value from `GetPickerInitialColor`. Clear handlers remove the explicit channel without overwriting lower-precedence data.

---

## Implementation references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)

For practical user workflows, see [Design](DESIGN.md). Source-reviewed behavior is distinguished from unexecuted Unreal Editor QA.
