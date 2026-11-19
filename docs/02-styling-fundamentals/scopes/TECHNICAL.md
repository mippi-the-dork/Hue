# Instance, Category and Global — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Scope enum versus UI naming

`EHueStyleScope` declares `Instance`, `Category`, and `GlobalFunction`; the last is the shared internal enum member for global identities. UI labels are derived by `GetGlobalScopeLabel` and can be Global Function, Global Variable, Global Event, Global Macro, Global Operator or Global Node.

## Resolution inputs

`GetBlueprintCategoryInfo` locates supported member categories in the defining Blueprint. `GetGlobalKey` generates a typed global identity appropriate to the K2 node family; it is not a naive visible-title comparison.

## Save ownership

`GetScopeBlueprint` selects the correct Blueprint for Instance/Category. `GetOrCreateBlueprintExtension` manages persistent Blueprint-owned `UHueBlueprintExtension` data. `UHueSettings::GlobalFunctionStyles` stores Global style rules in editor project config.

## Eligibility

`FHueStyleResolver::IsSupportedNode` is stricter than `CanStyleNode`: a style can only be edited after Hue confirms an actual visual path. Both the panel and context menu check scope eligibility before offering actions.

---

## Implementation references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)

For practical user workflows, see [Design](DESIGN.md). Source-reviewed behavior is distinguished from unexecuted Unreal Editor QA.
