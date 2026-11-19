# Right-Click Styling — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## ToolMenus registration

FHueModule::RegisterMenus and RegisterLoadedNodeContextMenus register graph contexts. RegisterContextMenuName accounts for different context menu names and module availability.

## Menu construction

FHueMenu::BuildNodeContextEntry, BuildHueMenu, BuildScopeMenu and AddChannelEntries assemble eligible submenu actions. OpenHueColorPicker connects the UI to the native picker.

## Shared resolver

Menu and panel use the same HueStyleResolver APIs for scope checks, target deduplication, transactions and saving. The menu is not a second styling format or persistence mechanism.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueMenu.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueMenu.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)

For editor usage, [read Design](DESIGN.md). Source-reviewed claims are not equivalent to completed UE editor QA.
