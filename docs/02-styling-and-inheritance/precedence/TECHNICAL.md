# Precedence and Effective Sources — Technical

**Hue v1.0.0 · C++ implementation**

## Per-channel resolver

`FHueStyleResolver::ResolveColor(Node, Channel, OutColor)` checks explicit overrides for a particular `EHueStyleChannel`. It consults `GetColorOverride`, Category chain resolution and the typed Global key before falling back to the native color.

`GetEffectiveCategoryColor` returns the effective color and the category that provided it; `bIncludeExact` controls whether the current exact Category is included before searching parents.

## Category traversal

Nested paths use Unreal's My Blueprint category syntax with `|` separators. The resolver searches from the most specific applicable path toward parents, and only for the requested channel. This means a nearer parent may own Header Color while a farther parent owns Body Color.

## Ownership reporting

`SHuePanel` computes source labels separately from the picker color. It compares explicit-override presence and inherited results across selected targets. It must distinguish equal colors with different ownership, because clearing them has different future behavior.

## Performance boundary

Category rename tracking is triggered by Blueprint change/compile events, not by scanning all category members during every color resolution or Slate paint. This avoids turning basic per-node tint evaluation into repeated Blueprint structure traversal.

[HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp) · [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp).

---

For user-facing workflows and examples, [read Design](DESIGN.md).
