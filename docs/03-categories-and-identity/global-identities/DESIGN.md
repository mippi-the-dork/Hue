# Global Node Identity — Design

**Hue v1.0.0 · User workflow**

## Global is not “every node with the same title”

Hue derives a logical key appropriate to each supported family, so repeated uses can share a project-wide visual meaning without accidentally styling unrelated nodes with similar text.

| Global identity | Typical effect |
| --- | --- |
| **Global Function** | Calls to the same function |
| **Global Variable** | Get and Set uses of the same Blueprint member variable |
| **Global Event** | Compatible uses of the same event identity |
| **Global Macro** | Uses of the same macro |
| **Global Operator** | A supported promotable operator regardless of promoted numeric type |
| **Global Node** | Supported node types that use a type-level identity when no more specific key applies |

<figure class="doc-media" data-kind="image" data-media-id="global-variable-get-set"><img src="../../assets/media-placeholder.png" alt="Hue Global Variable style shared by Get and Set nodes" loading="lazy"><figcaption>Variable Get and Set nodes can share the same Global Variable style because they represent one underlying member identity.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Show one Blueprint containing Get and Set nodes for the same named variable and an unrelated variable. Apply a recognizable Global Variable Header Color so Get and Set share the color while the unrelated variable remains unchanged. Keep the Hue panel source label visible.</span></figcaption></figure>

## Example: Global Variable

Give the Global Variable identity of `Health` a teal Header Color. Compatible Get Health and Set Health nodes use that Global rule. A red Instance Header Color on one Set node can still override it locally.

## Avoid broad assumptions

A Global style does not target every node with a word such as “Damage” in its visible title. A node may have no Global identity at all, in which case the scope is unavailable. Check the exact scope label in the panel before applying a bulk Global change.

## Team-wide usage

Use Global styles sparingly for stable recognizable identities shared across projects. Keep their meanings consistent and review changes to Hue's project settings when collaborating.

---

For implementation and storage, [read Technical](TECHNICAL.md).
