# Four Visual Channels — Design

**Hue v1.0.0 · User guide**

## One node, four independent decisions

| Channel | What changes | Caveat |
| --- | --- | --- |
| **Header Color** | Node title/header region | Compact nodes may have one central fill |
| **Header Text Color** | Primary title and compatible secondary title text | Native muted secondary-text behavior remains |
| **Body Color** | Main body below header | May act as compact-fill fallback when Header Color isn't overridden |
| **Body Text Color** | Supported pin labels, execution triangles and body text | Editable native fields remain Unreal-owned |

<figure class="doc-media" data-kind="image" data-media-id="four-channel-diagram"><img src="../../assets/media-placeholder.png" alt="Annotated Blueprint node displaying Hue's four visual channels" loading="lazy"><figcaption>The four Hue channels can be styled independently to distinguish a node's title, title text, body and pin labels.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture one standard K2 function-call node with a red header, white header text, dark-blue body and yellow body/pin-label text. Annotate all four regions and keep native value inputs recognizable.</span></figcaption></figure>

## A useful exercise

Apply contrasting values to all four channels of a supported standard call node. Then clear just Header Text Color. The other three remain overridden, while the title text returns to the next available inherited value.

## Compact nodes are different

An operator or other compact Blueprint node may not expose both distinct body and header surfaces. Header Color controls its main compact fill, and Body Color acts as a fallback if no stronger Header Color Hue rule exists. Avoid assuming every K2 node has the same Slate hierarchy.

## Native controls stay native

Hue deliberately does not recolor the text or chrome of numeric inputs, checkboxes, dropdowns, object/class pickers, and other native editing widgets merely because Body Text Color is set. This prevents styling from compromising their legibility and interaction.

---

For Unreal implementation, data ownership and boundaries, [read Technical](TECHNICAL.md).
