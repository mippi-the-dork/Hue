# Right-Click Styling — Technical

**Hue v1.0.0 · Source-level implementation**

## ToolMenus integration

`FHueModule::RegisterMenus`, `RegisterLoadedNodeContextMenus`, `RegisterContextMenuName` and `RefreshNodeContextMenus` attach Hue actions to eligible native K2 context menus. Module change handling supports contexts registered after initial startup.

## Target selection

`HueMenu.cpp` checks the right-clicked `ContextNode`, confirms `FHueStyleResolver::IsSupportedNode`, and assembles relevant selected compatible nodes if the right-click target is within the selected scope. Scope availability uses Category identity or `GetGlobalKey`.

## Commands and transactions

The context menu invokes `FHueStyleResolver::SetColorOverrides`, `ClearColorOverrides` and `ClearAllOverrides` for the applicable target set, rather than maintaining a separate style engine from the Hue panel. Native color-picker operations are confirmed before writing.

## Source

[HueMenu.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueMenu.cpp) · [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp).

---

For normal usage and examples, [read Design](DESIGN.md).
