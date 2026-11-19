# Updating Hue and Getting Support — Technical

**Hue v1.0.0 · Source-level reference**

## Module version and platform

[Hue.uplugin](https://github.com/mippi-the-dork/Hue/blob/main/Hue.uplugin) declares `Version:25`, `VersionName:1.0.0`, `EngineVersion:5.8.0`, an Editor module loaded PostEngineInit and `SupportedTargetPlatforms:[Win64]`. The README claims validation through 5.8.3; this manual does not claim validation on newer engines or other operating systems.

## Upgrade-safe sources

The plugin source and built binaries can change between releases; replace full distribution directories instead of overlaying files from unrelated releases. The Blueprint extension and `UHueSettings` are separate persistent data owners.

## Compatibility risks

UE editor APIs for graph node widgets, Blueprint editor tabs and ToolMenus may change between engine versions. When upgrading, verify `FEdGraphUtilities` factory registration, native Slate widget compatibility, tab registration and category rename tracking.

## Support triage

A graph display issue needs the exact `UK2Node` class and native presentation path; a state issue needs scope, channel and source label; a persistence issue needs the modified Blueprint/config owner and the save/reopen sequence. When opening a bug report include the Unreal Output Log and minimal reproduction if possible.

## Source references

[HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp) · [Hue.uplugin](https://github.com/mippi-the-dork/Hue/blob/main/Hue.uplugin) · [README.md](https://github.com/mippi-the-dork/Hue/blob/main/README.md).

---

For practical use, [read Design](DESIGN.md).
