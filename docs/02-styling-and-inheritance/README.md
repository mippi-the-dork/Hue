# 02. Styling and Inheritance

Hue exposes **four separately resolved visual channels** and **three scopes**. A color does not need to come from the same source as the other three colors. That is what makes a useful project-wide visual language possible without losing precise local exceptions.

## Choose a starting point

Use **Visual Channels** to understand which part of a node changes; **Styling Scopes** for where the rule lives; **Precedence** to understand the effective result; and **Clearing** to remove a local exception without wiping broader rules.

## Working example

A Global Function style supplies a blue body. A Category supplies a red header. A single Instance supplies yellow body text. Those can all coexist on one node because each channel resolves independently.

<figure class="doc-media" data-kind="image" data-media-id="channel-scopes-diagram">
  <img src="../assets/media-placeholder.png" alt="Hue four-channel style diagram" loading="lazy">
  <figcaption>Four independent color channels can resolve from different styling scopes on the same node.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture or draw a single Blueprint node with four contrasting visual channels and labels identifying the Instance, Category or Global source of each value.</span></figcaption>
</figure>

## Topics

| Topic | What you'll learn | Design | Technical |
| --- | --- | --- | --- |
| **Visual Channels** | Header and body color, text colors and compact node behavior | [Design](visual-channels/DESIGN.md) | [Technical](visual-channels/TECHNICAL.md) |
| **Styling Scopes** | Instance, user-defined Blueprint Category and Global styles | [Design](styling-scopes/DESIGN.md) | [Technical](styling-scopes/TECHNICAL.md) |
| **Precedence and Effective Sources** | Independent per-channel resolution and source labels | [Design](precedence/DESIGN.md) | [Technical](precedence/TECHNICAL.md) |
| **Clearing Overrides** | Clear one channel or a whole scope and reveal inherited values | [Design](clearing/DESIGN.md) | [Technical](clearing/TECHNICAL.md) |

The **Design** guide explains the user-facing workflow. The **Technical** guide describes source ownership, native engine integration and limitations. Use both when investigating a behavior that isn't immediately obvious.
