# Style Precedence and Effective Sources — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Per-channel resolution

<!-- Media target: IMAGE | resolution-lookup | docs/assets/media/resolution-lookup.png -->
<figure class="doc-media" data-kind="image" data-media-id="resolution-lookup">
  <img src="../../assets/media-placeholder.png" alt="Hue resolves each visual channel through a predictable precedence chain." loading="lazy">
  <figcaption>Hue resolves each visual channel through a predictable precedence chain.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Create a top-to-bottom flowchart Instance → exact Category → nearest parents → Global → Unreal Default, with an independent channel lookup label.</span></figcaption>
</figure>


`FHueStyleResolver::ResolveColor` checks the explicit Instance override, then `GetEffectiveCategoryColor` (exact and hierarchical category keys), then the applicable Global style. If none applies, native Unreal styling remains.

## Category distance

Nested category paths use Unreal's `|` separator. `GetEffectiveCategoryColor` can return the key that supplied the color through `OutSourceCategory`; nearest parents take priority before more distant ancestors.

## Source reporting

`SHuePanel::GetSingleSelectionSourceText` and `GetInheritedSourceTooltip` determine which rule to display as the origin. A scope panel's picker can display inherited/effective colors even when that scope has no explicit override.

## Caching and repaint

Style changes invalidate relevant Slate widgets. The resolver avoids repeated hierarchy scans during normal graph paint by keeping category rename migration event-driven rather than coupling it to every color lookup.

---

## Implementation references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)

For practical user workflows, see [Design](DESIGN.md). Source-reviewed behavior is distinguished from unexecuted Unreal Editor QA.
