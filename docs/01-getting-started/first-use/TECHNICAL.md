# First Use — Technical

**Hue v1.0.0 · C++ source reference**

## Selection to panel flow

`SHuePanel::GatherSelection` finds the Blueprint editor's selected graph nodes. `RefreshSelectionCache` and `BuildSelectionKey` help the panel detect meaningful selection and style changes before rebuilding native Slate controls.

## Scope gating

The panel checks `FHueStyleResolver::IsSupportedNode` first. The Category scope additionally calls `GetBlueprintCategoryInfo`; Global requires a nonempty `GetGlobalKey`. An unsupported node can stay selected without being edited.

## Color editing

`SHuePanel::OnSetColorClicked` opens a native `SColorPicker`; accepted values dispatch `SetColorOverrides` to the style resolver. A channel clear calls `ClearColorOverrides`; a scope clear uses `ClearAllOverrides`. The public batch API intentionally performs one transaction per action.

## Effective state display

For each channel, the panel distinguishes an explicit override from a value supplied by a parent Category, Global rule or Unreal default. In mixed selections the same displayed color need not imply identical source ownership.

## Source

[SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp) · [HueStyleResolver.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.h).

---

For normal user workflows, [read Design](DESIGN.md). Source-verified behavior is not a substitute for completed live editor QA.
