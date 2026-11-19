# Your First Styled Node — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Selection and scope evaluation

`SHuePanel::Tick` and `RefreshSelectionCache` detect changes to the current Blueprint graph selection. `GetLiveSelectedNodes` collects live graph nodes, while `FHueStyleResolver::IsSupportedNode` filters which nodes have a confirmed visual styling path.

## Scope eligibility

Instance applies to supported node instances. Category additionally calls `GetBlueprintCategoryInfo`. Global requires `GetGlobalKey` to return a usable identity. Unavailable scopes should not create fake categories or affect an unsupported node.

## Editing pathway

Panel picker callbacks invoke the batch-capable resolver methods, including `SetColorOverrides`, `ClearColorOverrides`, and `ClearAllOverrides`. These methods are responsible for the transaction and invalidation, not the Slate color swatch itself.

## Saving

Instance and Category use `UHueBlueprintExtension` stored on the defining/owning Blueprint. Global uses `UHueSettings` project config, where changes are saved as editor configuration. All visual effects remain editor-only.

---

## Implementation references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)

For practical user workflows, see [Design](DESIGN.md). Source-reviewed behavior is distinguished from unexecuted Unreal Editor QA.
