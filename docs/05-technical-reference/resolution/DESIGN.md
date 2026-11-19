# Style Identity and Color Resolution — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Same title does not mean same style

<!-- Media target: IMAGE | global-identity | docs/assets/media/global-identity.png -->
<figure class="doc-media" data-kind="image" data-media-id="global-identity">
  <img src="../../assets/media-placeholder.png" alt="Global styling groups nodes by logical identity rather than matching the text displayed on screen." loading="lazy">
  <figcaption>Global styling groups nodes by logical identity rather than matching the text displayed on screen.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture Get and Set nodes for the same member variable sharing Global Variable style, with a different variable showing another identity.</span></figcaption>
</figure>


Global styling follows a **logical identity**. Calls to one function generally share a Global Function style. Variable Get and Variable Set nodes for one member share a Global Variable style. Operators can use a stable operator identity independent of their promoted numeric type.

## Source hierarchy

For each of the four channels, Hue checks Instance, exact Category, nearest parent Categories, Global, then Unreal Default. The panel can show which layer actually contributed the displayed value.

## Why a style changes unexpectedly

If a parent Category is edited, descendants without closer overrides update. If a Global rule changes, nodes relying on that rule update even across different Blueprints. A new Instance override supersedes lower layers only on that one node.

## Confirm the owner

Select the affected node and inspect Hue's source labels. Clear a test override to see which lower-precedence rule appears. Avoid assigning a new color merely to cancel out a parent style if the correct fix is editing/clearing that parent rule.

## Limitations

Global identifiers are derived from node-specific semantics. Changing displayed labels or titles does not necessarily change the identity; nonmatching node identities may still show the same title.

---

For native implementation details, [read Technical](TECHNICAL.md).
