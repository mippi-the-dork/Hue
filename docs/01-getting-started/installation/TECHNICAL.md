# Installation and Compatibility — Technical

**Hue v1.0.0 · C++ source reference**

## Compatibility declarations

The v1.0.0 [Hue.uplugin](https://github.com/mippi-the-dork/Hue/blob/main/Hue.uplugin) descriptor uses `VersionName: 1.0.0`, `EngineVersion: 5.8.0`, `SupportedTargetPlatforms: Win64`, `CanContainContent: false` and `Type: Editor`. The release README states a tested engine range through 5.8.3. Other platforms and versions must be tested separately.

## Build dependencies

[Hue.Build.cs](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Hue.Build.cs) declares Editor and Slate dependencies including Blueprint editor/graph integration. Compile the project's **Editor** configuration; do not reference Hue as a packaged runtime dependency.

## Startup and shutdown

`FHueModule::StartupModule` registers the visual node and pin factories, Blueprint tabs/layout extensions and ToolMenus registration. `ShutdownModule` removes those hooks and releases tracked delegates. This integration is entirely plugin-side; no engine source patch is required.

## Diagnosing missing UI

Separate plugin load failure from Blueprint tab registration, graph node compatibility and scope applicability. A Hue tab can exist even if the currently selected node does not have an available Category or Global identity. See [Editor Architecture](../../05-technical-reference/architecture/TECHNICAL.md).

---

For normal user workflows, [read Design](DESIGN.md). Source-verified behavior is not a substitute for completed live editor QA.
