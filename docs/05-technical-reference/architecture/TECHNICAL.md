# Editor Integration Architecture — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Startup and shutdown

FHueModule::StartupModule registers HueNodeFactory and HuePinFactory with FEdGraphUtilities, ToolMenus callbacks and Blueprint Editor tab/layout extensions. ShutdownModule unregisters handles and factories.

## Graph presentation layers

FHueNodeFactory provides SHueGraphNode subclasses for supported K2 nodes. HueNativeNodeDecorator can attach to compatible native Slate widget surfaces, while HuePinFactory handles supported execution pin visuals.

## Scope and data layers

FHueStyleResolver provides global identity, category lookup, color precedence, batch mutations, and Blueprint change tracking. SHuePanel and FHueMenu are UI clients of this shared resolver.

## Confirmed visual path

CanStyleNode is a capability check, not proof a presentation has been decorated. MarkVisualSupport/IsSupportedNode establish the actual live supported state used by editor controls.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)
- [HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h)

For user-facing behavior, [read Design](DESIGN.md). Source inspection alone does not establish completed live Unreal Editor testing.
