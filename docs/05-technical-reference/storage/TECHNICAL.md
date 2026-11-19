# Saving and Project Configuration — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Blueprint extension storage

UHueBlueprintExtension (a UBlueprintExtension) declares InstanceStyles as TMap<FGuid,FHueNodeStyleOverride> and CategoryStyles as TMap<FString,FHueNodeStyleOverride>. The extension belongs to the Blueprint asset.

## Project config

UHueSettings is a UDeveloperSettings class with Config=Editor, DefaultConfig and the GlobalFunctionStyles config map. The property name is retained for compatibility even though the map stores several typed global identities.

## Write paths

HueStyleResolver saves Blueprint-backed scopes by marking their owning Blueprint for normal asset persistence. Global writes use UHueSettings and SaveConfig; changes invalidate Slate widgets so their visual effect appears without altering compile logic.

## Transactions

Batch changes use native FScopedTransaction behavior. Metadata, config and Blueprint asset saves have different ownership boundaries—do not describe all changes as stored in node colors or as a single project-wide file.

## Source control limitations

Hue does not implement locks/checkouts, conflict resolution or custom collaboration mechanisms.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)
- [HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h)

For user-facing behavior, [read Design](DESIGN.md). Source inspection alone does not establish completed live Unreal Editor testing.
