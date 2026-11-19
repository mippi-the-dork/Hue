# Instance, Category and Global Scopes — Technical

**Hue v1.0.0 · C++ implementation**

## Data scopes

`EHueStyleScope` in [HueTypes.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Public/HueTypes.h) contains `Instance`, `Category`, and `GlobalFunction`. The last enum name is retained in the C++ API, even though `GetGlobalKey` supports multiple kinds of Global node identity. Do not mistake it for function-only coverage.

## Blueprint-owned state

`UHueBlueprintExtension` stores `InstanceStyles` as `TMap<FGuid,FHueNodeStyleOverride>` and `CategoryStyles` as `TMap<FString,FHueNodeStyleOverride>`. Instance keys use `NodeGuid`; Category keys use exact My Blueprint Category strings associated with the defining Blueprint.

## Project-wide state

`UHueSettings::GlobalFunctionStyles` is a Config `TMap<FString,FHueNodeStyleOverride>`; its name is kept for configuration compatibility even as the key resolver includes more than function identities.

## Scope resolution

`FHueStyleResolver::GetScopeBlueprint`, `GetOwningBlueprint`, `GetDefiningBlueprint` and `GetBlueprintCategoryInfo` separate the owner of the displayed call node from the Blueprint that actually defines the relevant member Category. Context-menu and panel controls check target applicability individually.

[HueStyleResolver.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.h) · [HueSettings.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Public/HueSettings.h).

---

For user-facing workflows and examples, [read Design](DESIGN.md).
