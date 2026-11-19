# Multi-Selection and Mixed Values — Technical

**Hue v1.0.0 · Source-level implementation**

## Selection model

`SHuePanel::GatherSelection` gathers selected graph nodes and tracks supported versus unsupported items. `FHueStyleResolver::GetUniqueScopeTargetCount` computes deduplicated target counts for Category/Global and ordinary counts for eligible Instance identities.

## Deduplication

In batch `SetColorOverrides` and `ClearColorOverrides`, repeated Category paths and repeated Global keys are not written redundantly. This makes a selection of several uses of one function logically consistent for Global editing.

## Mixed-value computation

The panel considers not only effective `FLinearColor` but also whether the channel is explicitly overridden in each target. Two equal RGB values with different override bits are not necessarily the same underlying style state.

## Transaction boundary

The batch API creates one `FScopedTransaction` for the complete edit and performs necessary data-owner modifications and redraws once per action, avoiding a separate Undo entry for every selected node.

[SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp) · [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp).

---

For normal usage and examples, [read Design](DESIGN.md).
