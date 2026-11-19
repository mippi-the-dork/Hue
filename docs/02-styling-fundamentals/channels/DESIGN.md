# Four Visual Channels — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Four controls, four different jobs

| Channel | What changes | What stays native |
| --- | --- | --- |
| **Header Color** | Node title-bar fill; central fill on some compact nodes | Underlying node type and execution |
| **Header Text Color** | Main title and compatible secondary title lines | Unreal's native subdued text behavior |
| **Body Color** | Node body fill, or compact fallback when no header override is present | Native layout and user controls |
| **Body Text Color** | Compatible pin-label and body-text surfaces, including execution-pin triangles | Text-entry fields, class/object pickers, dropdowns and checkboxes |

A node can obtain each of these channels from different scopes.

## Example with strong contrast

Set Header Color red, Header Text Color white, Body Color dark blue, and Body Text Color yellow on a normal supported node. This deliberately exaggerated style makes each region easy to identify during setup.

## Why a channel might look unchanged

A compact node may have no separate visible body area. Some specialized node families retain native widgets that Hue does not recolor. If an editable value field remains its normal Unreal style, that's intentional; only supported label surfaces use Body Text Color.

## Useful practice

Start with **Header Color** for category meaning and use body/text overrides sparingly for local emphasis. Keep text contrast readable when establishing a shared graph color language.

---

For the underlying C++ implementation, see [Technical](TECHNICAL.md).
