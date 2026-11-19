# Multi-Selection and Mixed Values — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Change several nodes together

Select several compatible, unrelated nodes. Hue supports batch styling across types, functions and categories without requiring graph adjacency.

## Scope matters

**Instance** changes each selected compatible occurrence. **Category** changes each unique relevant category. **Global** changes each unique logical identity, even if selected nodes repeat that identity.

## What Multiple Values means

Different explicit colors, inherited values or underlying ownership can trigger **Multiple Values**. Two nodes can look identical but differ because one explicitly overrides a channel and the other inherits it.

## Try a reversible batch

Select several node types, set Instance Body Color, then Undo once. The whole set should revert as one transaction; Redo reapplies it.

## Unsupported nodes

Keep unsupported nodes selected if convenient. Hue reports them separately and skips them rather than trying to recolor an incompatible native widget.

## Important caution

Category or Global changes may affect other nodes beyond the current selection, because the new rule is shared. Inspect the scope before applying a batch.

---

For native implementation detail, [read Technical](TECHNICAL.md).
