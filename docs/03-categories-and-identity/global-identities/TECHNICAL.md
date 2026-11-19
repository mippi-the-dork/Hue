# Global Node Identity — Technical

**Hue v1.0.0 · Unreal implementation reference**

## Identity key generation

`FHueStyleResolver::GetGlobalKey` constructs typed keys for function, variable, event, macro, promotable operator and other supported nodes. Certain previously authored function keys remain raw `UFunction` paths for configuration compatibility; not all newer identity types use the same key representation.

## C++ naming legacy

`EHueStyleScope::GlobalFunction` and `UHueSettings::GlobalFunctionStyles` retain historic naming. The public label `GetGlobalScopeLabel` and tooltip identify the logical Global scope appropriate to the selected node.

## Storage

`UHueSettings` is `Config=Editor` and contains `GlobalFunctionStyles` as a `TMap<FString,FHueNodeStyleOverride>`. The resolver maps the typed key to its explicit override. Global writes call `Modify` where appropriate, update the map, and save config.

## Target deduplication

Batch edits aggregate unique Global keys rather than writing once per visible selected node. This matters when the selection contains several calls to the same function or both Get and Set of the same variable.

[HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp) · [HueSettings.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Public/HueSettings.h).

---

For usage and examples, [read Design](DESIGN.md).
