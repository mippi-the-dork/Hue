# Native Slate Rendering and Compatibility — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Preserving the node matters

Hue alters visual surfaces but doesn't replace a Blueprint node's computational behavior or standard editing controls. Header/body tint and compatible labels can change, while numeric input fields, checkboxes and object/class pickers remain Unreal-owned.

## Specialized node families

Events, timelines, switches, operators, variables and composites can have dedicated node presentations. Hue uses compatible wrappers or native decoration rather than treating every node as a basic function call.

## The supported-node boundary

A node has to be both logically eligible and visually compatible. Hue may decline to style a valid K2 node if its specialized widget cannot be safely decorated.

## Visual differences

For compact nodes with no separately painted body, Body Color may act as a fallback to Header Color. Subdued secondary title lines and disabled/unrelated node opacity should remain recognizable.

## If your custom node doesn't work

Test with a normal function call first. Then report the precise class and full native node screenshot. A safe extension would require a known visual path; Hue does not promise arbitrary recoloring of any unknown widget.

---

For native implementation details, [read Technical](TECHNICAL.md).
