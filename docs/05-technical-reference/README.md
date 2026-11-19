# 05. Technical Reference

**Hue v1.0.0 · Editor integration, data ownership, identity resolution and Slate rendering**

Hue's technical design separates **style data**, **logical target resolution**, and **native Blueprint graph presentation**. This lets styles stay persistent without changing the execution logic of the owning nodes.

## Core source modules

<!-- Media target: IMAGE | technical-editor-architecture | docs/assets/media/technical-editor-architecture.png -->
<figure class="doc-media" data-kind="image" data-media-id="technical-editor-architecture">
  <img src="../assets/media-placeholder.png" alt="Hue separates the rules that choose colors from the native Slate surfaces that display them." loading="lazy">
  <figcaption>Hue separates the rules that choose colors from the native Slate surfaces that display them.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Create a diagram of HueModule, HueStyleResolver, SHuePanel, HueMenu, HueNodeFactory, HuePinFactory, HueNativeNodeDecorator and Blueprint/config storage.</span></figcaption>
</figure>


HueStyleResolver identifies the scope target, resolves colors and applies transactions. HueModule integrates with Blueprint Editor tabs, ToolMenus and graph factories. SHuePanel and HueMenu are UI entry points. HueNodeFactory, HuePinFactory and HueNativeNodeDecorator handle compatible graph visuals.

## Data ownership

Per-instance and per-category overrides are stored through UHueBlueprintExtension on Blueprint assets. Global style identities live in UHueSettings editor configuration. Native graph node classes and their functionality remain Unreal-owned.

## Compatibility principle

Styling is offered only when a node has a confirmed compatible visual path. Known specialized nodes may use native decoration bridges instead of complete widget replacements. Unrecognized presentations are conservatively left unchanged.

## How to use this section

These pages are intended for technical users, diagnostics and plugin maintenance. For normal editing steps, the Design guides in Styling Fundamentals and Editing Workflows are the primary references.

## Topics in this section

| Topic | What you will learn | Design | Technical |
| --- | --- | --- | --- |
| **Editor Integration Architecture** | Understand how Hue registers graph UI, tabs and context-menu hooks | [Design](architecture/DESIGN.md) | [Technical](architecture/TECHNICAL.md) |
| **Saving and Project Configuration** | Find instance, category and global style storage | [Design](storage/DESIGN.md) | [Technical](storage/TECHNICAL.md) |
| **Style Identity and Color Resolution** | Trace scope resolution and logical global keys | [Design](resolution/DESIGN.md) | [Technical](resolution/TECHNICAL.md) |
| **Native Slate Rendering and Compatibility** | Learn supported node factories and safe native decoration | [Design](native-rendering/DESIGN.md) | [Technical](native-rendering/TECHNICAL.md) |

Read **Design** for user behavior and **Technical** for the source implementation. The guides complement one another rather than duplicating the same walkthrough.
