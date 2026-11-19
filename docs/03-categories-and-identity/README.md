# 03. Categories and Node Identity

Hue understands **My Blueprint** member Categories and stable logical identities. Those are not the same as Unreal's general action-search menu categories or a node's displayed text.

## Why this distinction matters

A category like `Combat|Weapons|Melee` can inherit a header color from `Combat|Weapons` and a body color from `Combat`. Global Function, Variable, Event, Macro and Operator identities can carry shared color meaning across uses. An Instance override remains local.

## Compatibility boundary

Hue only offers edit controls after it establishes a compatible visual presentation. Reroute and Documentation nodes are intentionally unsupported in v1.0.0. Native numeric fields, checkboxes and selectors continue to use Unreal's own widgets.

<figure class="doc-media" data-kind="image" data-media-id="category-tree">
  <img src="../assets/media-placeholder.png" alt="Hue nested Blueprint categories" loading="lazy">
  <figcaption>Nested My Blueprint Categories allow child nodes to inherit selected channels from their parents.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture My Blueprint with Combat, Combat|Weapons and Combat|Weapons|Melee (or an equivalent nested hierarchy) and a graph demonstrating contrasting inherited colors.</span></figcaption>
</figure>

## Topics

| Topic | What you'll learn | Design | Technical |
| --- | --- | --- | --- |
| **Nested Category Inheritance** | Parent categories and independent child overrides | [Design](nested-categories/DESIGN.md) | [Technical](nested-categories/TECHNICAL.md) |
| **Category Rename Migration** | How styled categories survive eligible Blueprint renames | [Design](category-renames/DESIGN.md) | [Technical](category-renames/TECHNICAL.md) |
| **Global Node Identities** | Function, variable, macro, event, operator and node identities | [Design](global-identities/DESIGN.md) | [Technical](global-identities/TECHNICAL.md) |
| **Supported Nodes and Native Controls** | Compatible K2 presentations and intentional unsupported cases | [Design](node-compatibility/DESIGN.md) | [Technical](node-compatibility/TECHNICAL.md) |

The **Design** guide explains the user-facing workflow. The **Technical** guide describes source ownership, native engine integration and limitations. Use both when investigating a behavior that isn't immediately obvious.
