# 01. Getting Started

Hue makes Blueprint graphs easier to scan by applying a consistent visual language to nodes without changing their logic. Its four visual channels can be styled at a single node, a Blueprint member Category, or a project-wide logical identity.

## The shortest useful workflow

Open a Blueprint, select a compatible node, open the **Hue** panel, expand **Instance**, and set **Header Color**. Watch just that node change. Then clear the override to restore inherited styling or the Unreal default. You do not need a new Actor, Blueprint subclass, or runtime system.

## What Hue does not do

Hue does not alter execution, compilation, runtime functionality or packaged rendering. It avoids replacing native editable controls such as number fields, dropdowns and object pickers, and it leaves visually unsupported node presentations alone.

<figure class="doc-media" data-kind="image" data-media-id="get-started-layout">
  <img src="../assets/media-placeholder.png" alt="Hue panel beside a selected Unreal Blueprint node" loading="lazy">
  <figcaption>Hue adds a dedicated Blueprint styling panel while preserving Unreal's normal graph editing tools.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture a Blueprint Editor with the Hue panel docked, one normal selected node, its graph visible, and clear Instance/Category/Global headings.</span></figcaption>
</figure>

## Topics

| Topic | What you'll learn | Design | Technical |
| --- | --- | --- | --- |
| **Overview** | Why Hue exists and when visual styling helps | [Design](overview/DESIGN.md) | [Technical](overview/TECHNICAL.md) |
| **Installation and Compatibility** | Fab, GitHub releases, source builds and supported Unreal environments | [Design](installation/DESIGN.md) | [Technical](installation/TECHNICAL.md) |
| **First Use** | Open the Hue panel, select a node and create a first override | [Design](first-use/DESIGN.md) | [Technical](first-use/TECHNICAL.md) |

The **Design** guide explains the user-facing workflow. The **Technical** guide describes source ownership, native engine integration and limitations. Use both when investigating a behavior that isn't immediately obvious.
