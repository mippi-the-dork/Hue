# Instance, Category and Global Scopes — Design

**Hue v1.0.0 · User guide**

## Three different places to save a style

| Scope | Affects | Best used for |
| --- | --- | --- |
| **Instance** | One node instance in its owning Blueprint | An exception with local meaning |
| **Category** | Applicable members in a user-defined My Blueprint Category | Shared styling for one feature/system |
| **Global** | Compatible uses of one logical node identity across project Blueprints | Repeated functions, variables, events, operators and macros |

<figure class="doc-media" data-kind="image" data-media-id="style-scope-comparison"><img src="../../assets/media-placeholder.png" alt="Hue Instance Category Global scope comparison" loading="lazy"><figcaption>The same Blueprint node can receive a broad Global color, a Category rule and an Instance exception.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Create three side-by-side Blueprint graph closeups featuring copies/calls of the same logical node. Label the Global rule that affects them all, the Category rule applied to one member group, and a final Instance-only exception with a distinct header color.</span></figcaption></figure>

## Example: Apply Damage

A project might assign a Global Function style to `Apply Damage`, allowing all calls to carry the same visual identity. In one Blueprint's `Combat` Category, a Category Header Color may override just the header. A single particularly dangerous call could then receive an Instance Body Text Color, without replacing its inherited body or header.

## How to choose a scope

Use **Global** when the node represents something consistent wherever used. Use **Category** when a node belongs to a system defined in My Blueprint. Use **Instance** when that one usage is unusual.

## Why Category or Global might be unavailable

Not every node represents a Blueprint-defined member Category, and some nodes have no applicable stable Global key. Hue disables the scope rather than pretending to edit all nodes with a similar visible label.

---

For engine implementation, ownership and limits, [read Technical](TECHNICAL.md).
