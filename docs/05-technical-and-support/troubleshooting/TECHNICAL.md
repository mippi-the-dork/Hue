# Troubleshooting and FAQ — Technical

**Hue v1.0.0 · Source-level reference**

## Troubleshooting by subsystem

| Symptom | Inspect |
| --- | --- |
| No Hue tab | `FHueModule::RegisterBlueprintTabs`, `HueTabSummoner` |
| No Hue context menu | `RegisterLoadedNodeContextMenus`, native menu registration timing |
| Node is shown but not editable | `IsSupportedNode`, native decorator visual bridge |
| Incorrect inherited color | `ResolveColor`, `GetEffectiveCategoryColor`, explicit override bits |
| Incorrect Global grouping | `GetGlobalKey`, node family and defining member identity |
| Category rename lost styling | Blueprint change tracking, migration conflict handling |
| Batch count unexpected | `GetUniqueScopeTargetCount`, deduplication by scope |
| Style not saved | Blueprint extension owner versus Hue editor config |

## No universal K2 widget guarantee

Registered graph factories cover known visual presentations, but some native-specialized nodes construct their own Slate widgets and require a separate decorator bridge. Hue conservatively avoids claiming full support if that bridge cannot bind safely.

## Debugging source reports

A displayed RGB does not identify whether the source is explicit or inherited. Compare `HasColorOverride` with the effective result, and check whether the node is backed by the defining Blueprint's Category map or a project-wide Global key.

## Verification

Test a simple call node, a member variable, nested Category with a parent override, and a mixed supported/unsupported selection. Execute Undo/Redo and close/reopen the owning Blueprint. No automatic passing result is inferred from source inspection.

[HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp) · [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp).

---

For practical use, [read Design](DESIGN.md).
