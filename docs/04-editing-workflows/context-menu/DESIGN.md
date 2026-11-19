# Right-Click Styling — Design

**Hue v1.0.0 · User guide**

## Style without leaving the graph

Right-click a compatible Blueprint node and open **Hue** in the node context menu. The available submenus reflect the selected node's applicable styling scopes—Instance, Category and its specific Global identity type.

<figure class="doc-media" data-kind="image" data-media-id="hue-context-menu"><img src="../../assets/media-placeholder.png" alt="Hue submenu in the native Unreal Blueprint node right-click menu" loading="lazy"><figcaption>Compatible Blueprint node context menus offer Hue styling commands for the scopes available on that node.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture a right-click menu on a supported function-call node with the Hue submenu open. Show the Instance, applicable My Blueprint Category and Global Function sections and at least one channel command. Include the node beneath the menu so its context is clear.</span></figcaption></figure>

## How to use it

1. Open a Blueprint graph and right-click a supported node.
2. Expand **Hue** and choose the appropriate scope.
3. Select the channel to set or clear.
4. Use Unreal's color chooser and confirm the result.
5. Undo if you want to reverse the action.

## Multi-selection behavior

If the node you right-click belongs to the current compatible multi-selection, the context menu can apply the action to the selection. If you right-click a different node, don't assume every previously selected node is affected. Check the actual selected nodes and available scope labels.

## When a submenu is missing

Category requires an applicable user-defined My Blueprint member Category; Global requires a recognized logical identity. Unsupported visual presentations have no appropriate Hue editing commands. An absence can be intentional, not necessarily a registration error.

## Panel versus context menu

The panel is best for comparing channels and effective source labels. The right-click menu is efficient when you already know the exact action and scope you want. They use the same underlying Hue style rules.

---

For the Unreal implementation, [read Technical](TECHNICAL.md).
