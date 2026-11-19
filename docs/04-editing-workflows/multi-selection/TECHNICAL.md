# Multi-Selection and Mixed Values — Technical

**Hue v1.0.0 · Source-level implementation**

## Selection model

<!-- Media target: IMAGE | batch-deduplication-tech | docs/assets/media/batch-deduplication-tech.png -->
<figure class="doc-media" data-kind="image" data-media-id="batch-deduplication-tech">
  <img src="../../assets/media-placeholder.png" alt="Hue deduplicates matching Category or Global targets before applying one transaction." loading="lazy">
  <figcaption>Hue deduplicates matching Category or Global targets before applying one transaction.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Draw five selected nodes collapsing into three unique category/global identities and one FScopedTransaction, while unsupported nodes are skipped.</span></figcaption>
</figure>


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
