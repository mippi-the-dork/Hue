# Hue Panel and Color Picker — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Find the panel

Open a Blueprint Editor, select a compatible graph node, and open its dockable **Hue** tab.

## The controls

The panel contains expandable Instance, Category and applicable Global scope sections. Each has Header Color, Header Text Color, Body Color and Body Text Color rows, plus color swatches, source-state information, and clearing controls.

## Style and inspect

1. Select one node.
2. Expand Instance and click the Header Color swatch.
3. Use Unreal's native color picker to set the desired tint.
4. Watch the graph update, then check the current source label.
5. Use Clear to remove the explicit Instance value and reveal whatever is inherited.

## Understanding selection states

Multiple selected nodes may show **Multiple Values** even if their visible colors match. A Category or Global section can be unavailable if selected nodes lack the matching identity. Unrecognized native nodes remain untouched.

## Why native fields don't recolor

Hue doesn't take control of editable numeric fields, dropdowns, checkboxes, class pickers or object pickers embedded inside nodes.

---

For native implementation detail, [read Technical](TECHNICAL.md).
