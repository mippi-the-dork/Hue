# Installation and Compatibility — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Descriptor and module configuration

`Hue.uplugin` specifies `FileVersion: 3`, `Version: 25`, `VersionName: 1.0.0`, `EngineVersion: 5.8.0`, and a single `Editor` module loaded at `PostEngineInit`. `CanContainContent` is false and the platform list contains Win64. It is not a packaged runtime module.

## Initialization order

`FHueModule::StartupModule` registers graph node and pin factories, then ToolMenus and Blueprint Editor hooks. Tab/layout callbacks install the Hue panel in compatible Blueprint Editors. Graph factory behavior is relevant only after the Editor module is actually loaded.

## Build configuration

`Source/Hue/Hue.Build.cs` lists Unreal Editor/Blueprint graph/Slate dependencies. Source builds need the target engine headers and a compatible compiler/toolchain; normal cooking targets should not require the Hue module.

## Upgrade compatibility

The current documentation represents v1.0.0 and the source checked on `main`. A Git checkout may move ahead of release binaries; don't mix claims about the two without comparing versions. No test is claimed passed solely because the plugin descriptor lists an engine version.

---

## Implementation references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)

For practical user workflows, see [Design](DESIGN.md). Source-reviewed behavior is distinguished from unexecuted Unreal Editor QA.
