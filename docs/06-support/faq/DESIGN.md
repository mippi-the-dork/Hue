# Frequently Asked Questions — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Does Hue affect gameplay?

No. Hue is an editor-only Blueprint presentation layer. Its visual colors don't alter Blueprint execution, compiler output or packaged-game rendering.

## Why didn't my variable Get and Set get separate Global colors?

They share the same underlying Global Variable identity. Use Instance styling if one occurrence needs to differ.

## Why is Category unavailable?

The node might not correspond to a user-authored My Blueprint member Category. A visible action-menu category is not sufficient.

## Why did clearing a color reveal another nondefault color?

Clear removes an explicit override. The next applicable Category, parent Category, Global or native fallback becomes visible.

## Why did editing Combat affect Combat|Weapons?

Nested categories inherit per channel from their nearest applicable parent.

## Why does a multi-selection show Multiple Values when nodes look identical?

Hue reports style ownership as well as appearance. One node may explicitly override a color that another inherits.

## Why does a Documentation or reroute node remain unchanged?

Both are intentionally unsupported in Hue v1.0.0.

## Does the right-click menu change the same data as the Hue panel?

Yes; both use the resolver and the same Instance, Category and Global style rules.

## How do I share colors with my team?

Save/submit affected Blueprint assets for Instance/Category styles, and share project Hue editor configuration for Global styles.

---

For native implementation details, [read Technical](TECHNICAL.md).
