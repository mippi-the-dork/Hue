# Hue Panel and Color Picker — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Panel lifecycle

FHueModule registers Blueprint Editor tabs through RegisterBlueprintTabs and extends layout through ExtendBlueprintLayout. SHuePanel is the actual native Slate panel.

## Selection refresh

SHuePanel::Tick, GatherSelection, BuildSelectionKey and RefreshSelectionCache detect new selection sets. RebuildPanel and BuildScopeSection create the currently applicable controls.

## Action handling

OnSetColorClicked launches the native color picker; OnClearColorClicked and OnClearScopeClicked call resolver mutations. GetOverrideStateText and GetSingleSelectionSourceText provide mixed and inherited source reporting.

## Target counting

GetScopeTargetCount reports unique identities, while GetEligibleNodeCount tracks how many compatible selected nodes can participate in a scope.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueMenu.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueMenu.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)

For editor usage, [read Design](DESIGN.md). Source-reviewed claims are not equivalent to completed UE editor QA.
