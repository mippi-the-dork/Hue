# Hue Panel — Technical

**Hue v1.0.0 · Unreal implementation reference**

## Construction and tab registration

`FHueModule::RegisterBlueprintTabs` uses a tab factory implemented in `HueTabSummoner`, integrating into the Blueprint editor layout rather than opening a separate application. `SHuePanel::Construct` initializes native Slate UI.

## Selection cache

`GatherSelection`, `BuildSelectionKey`, `RefreshSelectionCache`, and `RebuildPanel` drive context-dependent updates. The panel must avoid rebuilding its whole widget tree unnecessarily on every editor tick.

## Scope and channel controls

`HuePanelPrivate::IsScopeApplicable` checks supported node identity and the validity of Category/Global scope information. The swatch and source label are derived from explicit and effective style values. Picker callbacks call the batch style-resolver API.

## Undo and color chooser

The panel uses native `SColorPicker` and `SColorBlock` widgets; selection edits delegate transactions to `FHueStyleResolver`. Native color editing does not require a custom Blueprint node execution change.

## Source

[SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp) · [HueTabSummoner.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueTabSummoner.cpp) · [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp).

---

For usage and examples, [read Design](DESIGN.md).
