# 04. Editing Workflows

Hue has two complementary ways to style nodes: the dockable **Hue panel** in a Blueprint editor and **Right-click → Hue** on compatible graph nodes. Both can work with multiple selected nodes.

## A productive flow

Begin with a Category or Global rule so related nodes gain visual consistency. Add Instance overrides only to exceptional nodes. When you need a batch change, multi-select compatible nodes, inspect the unique target count and Multiple Values state, then apply the edit as one Undo transaction.

## When working with teams

Instance and Category rules travel with Blueprint assets; Global rules live in Hue's project editor configuration. Save and submit the appropriate files through your ordinary project source-control process.

<figure class="doc-media" data-kind="gif" data-media-id="panel-context">
  <img src="../assets/media-placeholder.png" alt="Hue Blueprint panel responding to selection" loading="lazy">
  <figcaption>The Hue panel updates as compatible Blueprint nodes are selected and styled.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record selecting two different compatible graph nodes and using the Hue panel to change an Instance color, then clear it. Keep source labels visible.</span></figcaption>
</figure>

## Topics

| Topic | What you'll learn | Design | Technical |
| --- | --- | --- | --- |
| **Hue Panel** | Selection-aware scope controls, pickers, sources and availability | [Design](hue-panel/DESIGN.md) | [Technical](hue-panel/TECHNICAL.md) |
| **Context Menu** | Right-click styling and context-sensitive scope options | [Design](context-menu/DESIGN.md) | [Technical](context-menu/TECHNICAL.md) |
| **Multi-Selection** | Batch edits, unique identities and Multiple Values | [Design](multi-selection/DESIGN.md) | [Technical](multi-selection/TECHNICAL.md) |
| **Undo, Saving and Teams** | Transactions, Blueprint packages, global settings and source control | [Design](undo-saving/DESIGN.md) | [Technical](undo-saving/TECHNICAL.md) |

The **Design** guide explains the user-facing workflow. The **Technical** guide describes source ownership, native engine integration and limitations. Use both when investigating a behavior that isn't immediately obvious.
