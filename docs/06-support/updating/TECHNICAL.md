# Updating, Reporting Bugs and Support — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Descriptor-level compatibility

Hue.uplugin reports VersionName 1.0.0, Win64 platform support, and a PostEngineInit Editor module against UE 5.8.0 baseline. README documents UE 5.8.0–5.8.3 compatibility; other versions/platforms aren't established by the manifest.

## Build and maintenance

Hue.Build.cs is an editor module dependency map. Changes to Slate graph APIs, FEdGraphUtilities factory registration, ToolMenus or Blueprint Editor tabs should be validated against the actual UE version.

## Stable serialized storage

UHueBlueprintExtension persists InstanceStyles/CategoryStyles within Blueprints. UHueSettings stores global rule data in Editor project config. Don't conflate plugin replacement with data deletion or promise untested migration support.

## Documentation updates

Whenever renderer, scope resolver, persistence or category rename behavior changes, update both Design and Technical documentation in the same change. Maintain version-specific source links where possible, and record real Unreal QA separately.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)
- [HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h)

For user-facing behavior, [read Design](DESIGN.md). Source inspection alone does not establish completed live Unreal Editor testing.
