# 01. Getting Started

**Hue v1.0.0 · Purpose, installation and first use**

Hue is an editor-only tool that makes **compatible Blueprint graphs easier to read**. It doesn't change Blueprint execution, packaged-game rendering, compiled output or native editor interactions.

## Your first workflow

<!-- Media target: IMAGE | getting-started-editor | docs/assets/media/getting-started-editor.png -->
<figure class="doc-media" data-kind="image" data-media-id="getting-started-editor">
  <img src="../assets/media-placeholder.png" alt="Hue lives alongside the Blueprint graph, giving compatible selected nodes a dedicated styling panel." loading="lazy">
  <figcaption>Hue lives alongside the Blueprint graph, giving compatible selected nodes a dedicated styling panel.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture UE 5.8 Blueprint Editor with graph, selected Call Function node, docked Hue tab, and Instance/Category/Global headings readable.</span></figcaption>
</figure>


Install Hue for the supported Unreal Editor, open a Blueprint, select a normal Call Function node and expand Instance in the Hue tab. Set its Header Color, inspect the visual change, then Clear the override. If a lower-level category or global rule exists, you will see it again.

## First concepts

Hue has four independent visual channels and three styling scopes. You don't need to understand the C++ implementation to use it; start with the Design guides. The corresponding Technical guides document ownership, resolution and limitations.

## Before using shared styles

Styles at Instance/Category scopes belong to the relevant Blueprint assets. Global rules belong to project Hue editor configuration. Review their different saving behavior before styling a team project.

## Topics in this section

| Topic | What you will learn | Design | Technical |
| --- | --- | --- | --- |
| **What Hue Does** | Understand why Hue changes presentation rather than Blueprint behavior | [Design](introduction/DESIGN.md) | [Technical](introduction/TECHNICAL.md) |
| **Installation and Compatibility** | Install from Fab, release or source and confirm the supported editor | [Design](installation/DESIGN.md) | [Technical](installation/TECHNICAL.md) |
| **Your First Styled Node** | Open the Hue tab, set a first color and read its source | [Design](first-use/DESIGN.md) | [Technical](first-use/TECHNICAL.md) |

Read **Design** for user behavior and **Technical** for the source implementation. The guides complement one another rather than duplicating the same walkthrough.
