# Undo, Persistence and Teams — Design

**Hue v1.0.0 · User guide**

## Save the right owner

Hue's styling follows its scope:

| Scope | Where it is stored | What to save/share |
| --- | --- | --- |
| **Instance** | The Blueprint asset that contains the node | That Blueprint asset/package |
| **Category** | The Blueprint that defines the member Category | The defining Blueprint asset/package |
| **Global** | Hue's project editor configuration | The relevant project's Hue configuration |

<figure class="doc-media" data-kind="image" data-media-id="hue-save-ownership"><img src="../../assets/media-placeholder.png" alt="Hue styling storage owner and source control diagram" loading="lazy"><figcaption>Instance and Category styling travel with Blueprint assets, while Global rules are maintained in project configuration.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Draw a three-lane diagram labeled Instance, Category and Global. Show the first two flowing to Blueprint assets/UHueBlueprintExtension and the third to project Hue editor config. Add Save Blueprint and Submit Config labels as appropriate.</span></figcaption></figure>

## A reliable team workflow

1. Agree on a shared project-wide palette before changing many Global rules.
2. Apply broad Global/Category rules, then local Instance exceptions.
3. Save the Blueprint assets that own Instance/Category changes.
4. Include the Hue project configuration when you intend to share Global rules with teammates.
5. Review conflicting style changes through your project's existing source-control workflow.

## Undo/Redo

Hue uses normal Unreal transactions. Setting or clearing a color across a selection is one Undo step. Undo reverses the authored change; it doesn't delete unrelated Blueprint graph changes.

## Persistence versus visual resolution

A style may be correctly saved but not currently visible because a higher-priority Instance/Category rule overrides it. Check effective source labels before assuming the underlying Global rule failed to persist.

## Limitations

Hue does **not** provide a Perforce or Git checkout system, permission control or automatic conflict resolution. Native Blueprint behavior and packaged-game output remain unaffected by style metadata.

---

For the Unreal implementation, [read Technical](TECHNICAL.md).
