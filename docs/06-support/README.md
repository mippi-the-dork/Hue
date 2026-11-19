# 06. Support and Troubleshooting

**Hue v1.0.0 · Common questions, issue diagnosis and update guidance**

Most Hue support problems fall into three groups: **a control or scope is unavailable**, **a node's effective color isn't what you expected**, or **a style didn't persist**.

## Start with the source label

<!-- Media target: IMAGE | support-effective-source | docs/assets/media/support-effective-source.png -->
<figure class="doc-media" data-kind="image" data-media-id="support-effective-source">
  <img src="../assets/media-placeholder.png" alt="Effective-source labels identify why a selected node has its current color." loading="lazy">
  <figcaption>Effective-source labels identify why a selected node has its current color.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture one selected node with two Hue channel rows showing distinct sources such as Override and Parent Category, with the node visible.</span></figcaption>
</figure>


If Clear didn't restore Unreal's original color, inspect the inherited Category or Global source. If a child changed when its parent changed, that's normal per-channel inheritance.

## Validate your node

Try an ordinary Call Function node. Documentation and reroute nodes are intentionally unsupported in v1.0.0, and certain specialized native widgets may remain untouched.

## Saving and teamwork

Instance and Category changes should be saved with the appropriate Blueprint asset. Global styling belongs to project Hue configuration. Confirm those files are included in your team's source-control policy.

## Reporting a useful issue

Record the exact UE and Hue versions, Blueprint node class, selected scope/channel, selection count, reproduction steps, and an un-cropped screenshot where presentation matters. Submit reports through the repository Issues page.

## Topics in this section

| Topic | What you will learn | Design | Technical |
| --- | --- | --- | --- |
| **Frequently Asked Questions** | Quick answers about categories, inheritance, saving and scope | [Design](faq/DESIGN.md) | [Technical](faq/TECHNICAL.md) |
| **Troubleshooting Hue** | Diagnose missing panels, unavailable scopes and unexpected colors | [Design](troubleshooting/DESIGN.md) | [Technical](troubleshooting/TECHNICAL.md) |
| **Updating, Reporting Bugs and Support** | Update safely and report reproducible issues | [Design](updating/DESIGN.md) | [Technical](updating/TECHNICAL.md) |

Read **Design** for user behavior and **Technical** for the source implementation. The guides complement one another rather than duplicating the same walkthrough.
