# 04. Editing Workflows

**Hue v1.0.0 · Hue panel, context menus, multi-selection and compatible nodes**

You can use Hue in two complementary ways: **the dockable Hue tab** for inspection and careful editing, or a **right-click Hue submenu** for quick actions.

## For one node

<!-- Media target: IMAGE | editing-workflows-overview | docs/assets/media/editing-workflows-overview.png -->
<figure class="doc-media" data-kind="image" data-media-id="editing-workflows-overview">
  <img src="../assets/media-placeholder.png" alt="Hue's panel and node context menu provide complementary ways to apply the same style rules." loading="lazy">
  <figcaption>Hue's panel and node context menu provide complementary ways to apply the same style rules.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture a selected function node, Hue panel and open right-click Hue submenu without obscuring important controls.</span></figcaption>
</figure>


Select it, open the desired scope and edit one of the four channels. Check the effective source to understand whether that channel is explicit or inherited.

## For many nodes

Select several compatible graph nodes, then use Instance, Category or Global deliberately. Instance acts on each occurrence; Category and Global deduplicate shared identities. Mixed values are reported rather than silently guessed.

## Native widget safety

A specialized Blueprint node may be visually compatible or intentionally unsupported. Hue preserves normal editable controls such as numeric fields and object pickers, rather than forcing every custom node into a generic replacement widget.

## Best starting point

Begin with the Hue Panel guide, then use Multi-Selection and Node Compatibility when you need to style a more complicated graph.

## Topics in this section

| Topic | What you will learn | Design | Technical |
| --- | --- | --- | --- |
| **Hue Panel and Color Picker** | Use the dockable panel, native picker and effective-source information | [Design](hue-panel/DESIGN.md) | [Technical](hue-panel/TECHNICAL.md) |
| **Right-Click Styling** | Apply the same Hue scopes directly from Blueprint node menus | [Design](context-menu/DESIGN.md) | [Technical](context-menu/TECHNICAL.md) |
| **Multi-Selection and Mixed Values** | Batch edit compatible nodes with one transaction | [Design](multi-selection/DESIGN.md) | [Technical](multi-selection/TECHNICAL.md) |
| **Supported and Unsupported Node Presentations** | Understand safe native styling and known exclusions | [Design](node-compatibility/DESIGN.md) | [Technical](node-compatibility/TECHNICAL.md) |

Read **Design** for user behavior and **Technical** for the source implementation. The guides complement one another rather than duplicating the same walkthrough.
