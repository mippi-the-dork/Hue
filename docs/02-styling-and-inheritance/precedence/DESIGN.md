# Precedence and Effective Sources — Design

**Hue v1.0.0 · User guide**

## Which color wins?

Each visual channel resolves through the same sequence, **independently of the other three**:

1. Instance override
2. Exact My Blueprint Category
3. Nearest parent Category
4. Successively higher parent Categories
5. Global logical identity
6. Unreal default

The first available explicit value wins. An absent override allows the search to continue. A value explicitly set to a color that resembles the default still counts as an override.

<figure class="doc-media" data-kind="gif" data-media-id="style-precedence-gif"><img src="../../assets/media-placeholder.png" alt="Hue visual style precedence and clear demonstration" loading="lazy"><figcaption>A Hue node immediately adopts the next applicable Category or Global value when a more specific override is cleared.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a supported function-call node with a Global blue Header Color, then apply a Category orange header and an Instance red header. Clear the Instance override to reveal orange; clear Category to reveal blue. Keep the Hue source label visible throughout.</span></figcaption></figure>

## Example: mixed ownership

Suppose a node resolves to the following:

| Channel | Visible color | Effective source |
| --- | --- | --- |
| Header Color | Red | Category: Combat |
| Header Text Color | Native | Unreal Default |
| Body Color | Dark Gray | Global Function |
| Body Text Color | Yellow | Instance |

There is no requirement for all four visible colors to come from the same scope.

## Reading the source labels

The Hue panel can report **Override**, `Category: Combat|Weapons`, `Parent: Combat`, the applicable Global type (Function/Variable/Event/Macro/etc.), or **Unreal Default**. In a multi-selection it may show **Multiple Values** even if nodes happen to render similarly: ownership itself can differ.

## Clearing a style

Clear only the channel you want to stop overriding. The next applicable value becomes visible. To reset a node all the way to the Unreal default, you may need to clear applicable Instance, Category and Global rules—not just one Instance color.

---

For engine implementation, ownership and limits, [read Technical](TECHNICAL.md).
