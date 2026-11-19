# Clearing Individual and All Overrides — Technical

**Hue v1.0.0 · C++ implementation**

## Style storage semantics

`FHueNodeStyleOverride` has one `bOverride...` bit per channel. `ClearColor` clears that bit for the specified channel; it does not write a special default color. When all bits are false, `IsEmpty()` is true and empty style records can be removed from their owning map.

## Transactions

`ClearColorOverrides(const TArray<UEdGraphNode*>&,Scope,Channel)` and `ClearAllOverrides(Nodes,Scope)` perform batch operations in a single `FScopedTransaction`, deduplicate scope keys, and trigger the relevant data-owner updates and graph redraw/invalidation.

## Owner-dependent paths

Instance Clear updates the owning Blueprint extension entry keyed by `NodeGuid`. Category Clear modifies the defining Blueprint extension's exact category key. Global Clear updates `UHueSettings::GlobalFunctionStyles` and persists project editor configuration using `SaveConfig`.

## Scope availability

The resolver ignores unsupported nodes or those with no applicable Category or Global identity. A mixed selection can still produce one coherent transaction on eligible scope targets.

[HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp) · [HueTypes.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Public/HueTypes.h).

---

For user-facing workflows and examples, [read Design](DESIGN.md).
