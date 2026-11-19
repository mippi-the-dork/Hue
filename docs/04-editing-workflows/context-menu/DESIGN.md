# Right-Click Styling — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Fast access from the graph

Right-click a supported node and open the Hue submenu in its normal Blueprint context menu. This offers Instance, Category, and applicable Global scopes without requiring the Hue panel to stay open.

## Available actions

Use the scope's channel entries to set a header/body or text color, or clear existing overrides. The available Global label varies by node identity: Function, Variable, Event, Macro, Operator or Node.

## Editing the selection

If the right-clicked node is part of a compatible multi-selection, an action can apply across the eligible selected targets. Repeated uses of one Global identity are deduplicated.

## Panel versus menu

The panel is best for source inspection, inheritance and multi-selection status. The menu is best for quick access to a known action.

## If Hue isn't shown

Test a standard Call Function node. Unavailable menus on a custom or specialized node may reflect unsupported presentation rather than a broken plugin installation.

---

For native implementation detail, [read Technical](TECHNICAL.md).
