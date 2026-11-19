# Clearing Individual and All Overrides — Design

**Hue v1.0.0 · User guide**

## Clear is not the same as reset to default

Each scope offers **Clear** for a channel and **Clear All Overrides** for the entire scope. Clear removes the current scope's explicit value. It does not erase broader rules or force a specific white/gray color.

<figure class="doc-media" data-kind="gif" data-media-id="clear-channels-gif"><img src="../../assets/media-placeholder.png" alt="Hue Clear Color and Clear All Overrides actions" loading="lazy"><figcaption>Clearing an Instance color reveals the inherited Category color without affecting the node's other channels.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a node with a Category green Header Color and an Instance red Header Color plus an Instance yellow Body Text Color. Clear only Instance Header Color; show the header turning green while yellow body text stays. Then use Clear All Overrides and show the remaining Instance body text reverting.</span></figcaption></figure>

## Example

Imagine Header Color is Red at Instance scope, Orange at Category scope, and Blue at Global scope. Clearing **Instance Header Color** reveals Orange. Clearing the applicable Category override next reveals Blue. Clearing the Global override finally reveals Unreal's native header color.

## Clear one channel or all four?

Use **Clear** when just one visual property should return to inheritance. Use **Clear All Overrides** when you want the chosen scope to stop controlling every visual channel, while retaining parent/Global rules.

## Undo works

Hue uses Unreal's transaction system. One multi-selection Clear or Clear All operation is one transaction, so you can reverse an accidental batch with Unreal Undo.

## Common confusion

If Clear appears not to change the visual color, a lower-precedence rule may provide the same color. Check the source label, not only the swatch. For a batch selection, ensure the scope actually has eligible unique targets.

---

For engine implementation, ownership and limits, [read Technical](TECHNICAL.md).
