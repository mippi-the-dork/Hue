# Your First Styled Node — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Your first edit

1. Open a Blueprint with an ordinary compatible **Call Function** node.
2. Select the node in the graph. Open the **Hue** tab.
3. Expand **Instance**; use **Header Color** to pick an easily distinguishable color.
4. Observe that only this node instance changes—not other calls to the same function.
5. Change **Body Text Color** separately. Header text and body tint should remain independent.
6. Use **Clear** on the Header Color. If a Category or Global style applies, that inherited value becomes visible; otherwise Unreal's default returns.
7. Undo and Redo a change using Unreal's normal history.

## First try with several nodes

Select two or three compatible, different nodes. The Hue panel reports the number of supported selection targets and may show **Multiple Values**. Set Instance Body Color once to apply it to all eligible nodes in a single transaction.

## Try a Category style

Choose a function/variable whose **My Blueprint Category** is assigned. Select its compatible node, expand Category, and set Header Color. Other compatible nodes for members in that same user-defined category can inherit the style.

## What to watch for

A scope that says **Unavailable** may simply lack the required identity. **Global** is based on a supported logical node identity, not matching visible titles. Unsupported nodes stay untouched even when included in a multi-selection.

## Before team use

Review [Saving and Project Configuration](../../05-technical-reference/storage/DESIGN.md). Instance and Category rules travel with Blueprint assets, while Global rules live in project Hue configuration.

---

For the underlying C++ implementation, see [Technical](TECHNICAL.md).
