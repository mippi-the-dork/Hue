# 05. Technical and Support

Hue is an **Editor** module for UE 5.8.x on Win64. It integrates with graph presentation and the Blueprint editor without changing the executable behavior of Blueprint nodes.

## What the implementation owns

The module registers graph-node and pin factories, the Blueprint panel tab, context-menu extensions and change/compile tracking. The style resolver maps nodes to Instance, Category and Global rule sources; Slate presentation adapters apply compatible visual colors without replacing native controls.

## Where to begin debugging

For wrong colors, check precedence and source labels. For no Hue control, check actual supported-node eligibility. For persistence, determine whether the change was Instance, Category or Global and save/commit the corresponding data owner.

<figure class="doc-media" data-kind="image" data-media-id="module-architecture">
  <img src="../assets/media-placeholder.png" alt="Hue Editor architecture diagram" loading="lazy">
  <figcaption>Hue connects Blueprint editing tools, style resolution and native-safe node presentation through editor-only interfaces.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Draw a diagram showing HueModule, HueStyleResolver, HuePanel, HueMenu, HueNodeFactory/PinFactory and native decorators, with Blueprint assets and project configuration as data owners.</span></figcaption>
</figure>

## Topics

| Topic | What you'll learn | Design | Technical |
| --- | --- | --- | --- |
| **Editor Architecture** | Graph node factories, decorators, Slate widgets and module lifecycle | [Design](architecture/DESIGN.md) | [Technical](architecture/TECHNICAL.md) |
| **Data Storage and Reconciliation** | Blueprint extension state, project config, rename tracking and redraw | [Design](storage/DESIGN.md) | [Technical](storage/TECHNICAL.md) |
| **Troubleshooting and FAQ** | Common styling, selection, inheritance and compatibility problems | [Design](troubleshooting/DESIGN.md) | [Technical](troubleshooting/TECHNICAL.md) |
| **Updating and Support** | Version compatibility, upgrades, bug reports and support channels | [Design](updating/DESIGN.md) | [Technical](updating/TECHNICAL.md) |

The **Design** guide explains the user-facing workflow. The **Technical** guide describes source ownership, native engine integration and limitations. Use both when investigating a behavior that isn't immediately obvious.
