# Multi-Selection and Mixed Values — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Unique representatives

GetUniqueScopeTargetCount and the resolver's internal GetUniqueScopeRepresentatives deduplicate by node instance, Blueprint category identity or global key as appropriate.

## Mutation batching

SetColorOverrides, ClearColorOverrides and ClearAllOverrides perform one transaction and group persistence/invalidation rather than issuing repeated writes per duplicate identity.

## Mixed-state logic

SHuePanel::GetOverrideStateText and GetSwatchColor compare selected scope values and ownership. Unsupported nodes are excluded through IsSupportedNode and reported separately.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueMenu.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueMenu.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)

For editor usage, [read Design](DESIGN.md). Source-reviewed claims are not equivalent to completed UE editor QA.
