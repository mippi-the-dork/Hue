# Troubleshooting Hue — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Troubleshooting by layer

**Startup:** inspect HueModule registration of node/pin factories, ToolMenus and Blueprint Editor tabs.

**Eligibility:** check HueStyleResolver::IsSupportedNode and GetBlueprintCategoryInfo or GetGlobalKey before diagnosing picker or color rendering.

**Visual bridge:** inspect HueNodeFactory, HuePinFactory and HueNativeNodeDecorator for specialized native widget support and fallback rejection.

**Style computation:** ResolveColor and per-channel source-reporting functions distinguish explicit, inherited and default values.

**Persistence:** check UHueBlueprintExtension on the actual defining Blueprint and UHueSettings for project-level global rules.

## Test boundaries

Reproduce with one known supported native K2 node before testing complex mixed selection, custom widgets, category migration or Blueprint recompilation. Log observed results rather than marking source review as successful runtime/editor QA.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)
- [HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h)

For user-facing behavior, [read Design](DESIGN.md). Source inspection alone does not establish completed live Unreal Editor testing.
