# Editor Architecture — Technical

**Hue v1.0.0 · Source-level implementation**

## Module startup

`FHueModule::StartupModule` registers `FEdGraphUtilities` visual node/pin factories, Blueprint tab registration, layout extensions, ToolMenus, and Blueprint change tracking. `ShutdownModule` unregisters callbacks and factories.

## Node presentation routes

`FHueNodeFactory` maps recognized K2 node types to appropriate `SHueGraphNode*` subclasses; `FHueNativeNodeDecorator` can bind to already native-presented widget surfaces where a custom replacement would risk losing specialized controls. `FHuePinFactory` preserves native pin widget behavior while styling compatible surfaces.

## Source-of-truth resolution

`FHueStyleResolver` reads explicit override bits from the appropriate Blueprint extension or project settings, resolves each `EHueStyleChannel`, and supplies colors to paint-time binding.

## Change lifecycle

Blueprint open, change and compiled callbacks maintain Category rename tracking. New style writes use transactions and invalidate relevant graph/widget presentation. A potentially supported node becomes panel-editable only after `IsSupportedNode` confirms the visual path.

## Source map

[HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp) · [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp) · [HueNativeNodeDecorator.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNativeNodeDecorator.cpp) · [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp).

---

For normal usage and examples, [read Design](DESIGN.md).
