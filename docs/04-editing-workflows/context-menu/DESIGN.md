# Right-Click Styling — Design

**Hue v1.0.0 · User guide**

## Style without leaving the graph

<!-- Media target: IMAGE | context-menu-hue | docs/assets/media/context-menu-hue.png -->
<figure class="doc-media" data-kind="image" data-media-id="context-menu-hue">
  <img src="../../assets/media-placeholder.png" alt="The Hue submenu offers scope-specific styling without requiring the docked panel." loading="lazy">
  <figcaption>The Hue submenu offers scope-specific styling without requiring the docked panel.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Right-click a node with Instance, Category and Global Function identities; capture the expanded Hue submenu with scope labels and channel commands.</span></figcaption>
</figure>


Right-click a compatible Blueprint node and open **Hue** in the node context menu. The available submenus reflect the selected node's applicable styling scopes—Instance, Category and its specific Global identity type.


## How to use it

1. Open a Blueprint graph and right-click a supported node.
2. Expand **Hue** and choose the appropriate scope.
3. Select the channel to set or clear.
4. Use Unreal's color chooser and confirm the result.
5. Undo if you want to reverse the action.

## Multi-selection behavior

<!-- Media target: GIF | context-menu-demo | docs/assets/media/context-menu-demo.gif -->
<figure class="doc-media" data-kind="gif" data-media-id="context-menu-demo">
  <img src="../../assets/media-placeholder.png" alt="Hue context actions apply through the same style system as the panel." loading="lazy">
  <figcaption>Hue context actions apply through the same style system as the panel.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record right-clicking a supported node, choosing Hue → Instance → Header Color, selecting a tint, and showing the matching change in the Hue panel.</span></figcaption>
</figure>


If the node you right-click belongs to the current compatible multi-selection, the context menu can apply the action to the selection. If you right-click a different node, don't assume every previously selected node is affected. Check the actual selected nodes and available scope labels.

## When a submenu is missing

Category requires an applicable user-defined My Blueprint member Category; Global requires a recognized logical identity. Unsupported visual presentations have no appropriate Hue editing commands. An absence can be intentional, not necessarily a registration error.

## Panel versus context menu

The panel is best for comparing channels and effective source labels. The right-click menu is efficient when you already know the exact action and scope you want. They use the same underlying Hue style rules.

---

For the Unreal implementation, [read Technical](TECHNICAL.md).
