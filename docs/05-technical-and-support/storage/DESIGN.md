# Data Storage and Reconciliation — Design

**Hue v1.0.0 · User guide**

## Where does Hue remember your colors?

Hue stores visual organization in the same place as the rule's scope:

- **Instance:** stored with the owning Blueprint and tied to the node's persistent GUID.
- **Category:** stored with the Blueprint that defines the user-created My Blueprint Category.
- **Global:** stored in the project's Hue editor settings.

<figure class="doc-media" data-kind="image" data-media-id="hue-style-ownership-chart"><img src="../../assets/media-placeholder.png" alt="Hue data storage split between Blueprint assets and editor settings" loading="lazy"><figcaption>Hue stores local node styles in Blueprint assets and project-wide Global styles in its editor configuration.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture or diagram an open Blueprint showing one selected Instance override and a Category override, plus Project Settings → Plugins → Hue showing Global Styles. Label which change writes the Blueprint asset versus project configuration.</span></figcaption></figure>

## How to avoid losing work

Save the appropriate Blueprint after applying Instance or Category changes. For Global changes, include the Hue project configuration in the files your team shares. Check whether an uncommitted higher-priority rule is hiding the effect of a saved lower-priority rule.

## Category rename persistence

When an applicable My Blueprint Category is renamed, Hue can migrate its existing Category style keys and those of its nested descendants. This process is not the same as moving one member to a new Category.

## Does Undo undo an entire batch?

Yes, authored styling actions use Unreal's transaction system. A batch color edit or Clear All in one scope is a single transaction, not one transaction per node.

## Source-control caution

Hue doesn't check files out, manage read-only files or resolve concurrent edits. Its project configuration and Blueprint assets must be saved and shared through your own Perforce/Git workflow.

---

For code and implementation, [read Technical](TECHNICAL.md).
