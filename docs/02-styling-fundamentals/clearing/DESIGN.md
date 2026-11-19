# Clearing Overrides and Undo — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Clear a single channel

<!-- Media target: GIF | clear-channel | docs/assets/media/clear-channel.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="clear-channel">
  <img src="../../assets/media-placeholder.png" alt="Clearing a specific color reveals the next applicable inherited rule without affecting other channels." loading="lazy">
  <figcaption>Clearing a specific color reveals the next applicable inherited rule without affecting other channels.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record Header Color set at Global, Category and Instance; clear Instance, then Category, then Global, showing each effective color/source change.</span></figcaption>
</figure>


Expand Instance, Category or Global and select **Clear** beside a particular channel. This removes only the explicit rule at that scope; the other three channels remain untouched.

For example, with red Instance Header, orange Category Header, and blue Global Header, clearing the Instance rule reveals orange. Clearing Category next reveals blue. Clearing Global finally restores Unreal's default.

## Clear everything at a scope

**Clear All Overrides** removes all four explicit channel overrides at the chosen scope. It does not erase rules belonging to other scopes. Use it when you no longer want a node or category to define a special style.

## Undo and Redo

Hue uses native Unreal transactions. A batch edit is a **single** transaction, so one Undo can restore the previous state across all affected selected nodes or categories. Use Redo when you want to reapply it.

## What not to expect

Clear is **not** a “set color to Unreal default” command. It removes an override and may reveal another inherited color. If the visual result appears unchanged, check the **effective source** label and whether the same value exists at a lower scope.

## Working with Global rules

Global clearing alters the project-wide style for that logical identity. Confirm whether that was your intention before clearing the rule on multiple node types.

---

For the underlying C++ implementation, see [Technical](TECHNICAL.md).
