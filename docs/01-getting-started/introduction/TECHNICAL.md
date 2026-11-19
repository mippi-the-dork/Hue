# What Hue Does — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Plugin identity and supported environment

`Hue.uplugin` declares an **Editor** module named `Hue`, with `PostEngineInit` loading, `Win64` support and Unreal `EngineVersion: 5.8.0`. The current descriptor reports **VersionName 1.0.0**. The project README identifies UE 5.8.0–5.8.3 as its validated release range.

## Main integration surfaces

`FHueModule::StartupModule` registers a `FGraphPanelNodeFactory`, a `FGraphPanelPinFactory`, ToolMenus support, Blueprint Editor tab registration, and layout extension callbacks. Visual replacement and native-widget decoration are distinct paths, not a blanket replacement of every `SGraphNode`.

## Domain model

`FHueNodeStyleOverride` contains four override-enabled flags and their `FLinearColor` values. `EHueStyleChannel` names the four channels; `EHueStyleScope` names Instance, Category, and `GlobalFunction` (the internal generic Global scope enum label retained in source).

## Scope of implementation

Styles reside in Blueprint extension data and project-level Hue settings; they do not change K2 compiler semantics. The runtime target does not load the Hue Editor module, and Hue does not introduce game Actors or components.

---

## Implementation references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)

For practical user workflows, see [Design](DESIGN.md). Source-reviewed behavior is distinguished from unexecuted Unreal Editor QA.
