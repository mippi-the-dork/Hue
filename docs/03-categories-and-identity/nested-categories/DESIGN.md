# Nested My Blueprint Category Inheritance — Design

**Hue v1.0.0 · User guide**

## Parent Categories are a color hierarchy

Hue understands My Blueprint member Category paths such as:

```text
Combat
Combat|Weapons
Combat|Weapons|Melee
```

You can style the parent `Combat` and let descendant Categories inherit its values automatically, then override only what a child needs.

<figure class="doc-media" data-kind="gif" data-media-id="nested-category-colors"><img src="../../assets/media-placeholder.png" alt="Hue nested Category inheritance updating on graph nodes" loading="lazy"><figcaption>Descendant Blueprint Categories inherit parent style channels until a closer Category supplies an override.<span class="media-capture-instructions"><strong>Capture instructions · GIF:</strong> Record a Blueprint with My Blueprint Categories Combat, Combat|Weapons and Combat|Weapons|Melee visible alongside matching graph nodes. Set Combat Header Color to red, then override Weapons Header Color with orange, and finally clear Weapons; show Melee inherit red again while unrelated channels remain unchanged.</span></figcaption></figure>

## Independent inheritance example

- `Combat` supplies **Header Color = Red** and **Body Color = Dark Gray**.
- `Combat|Weapons` overrides **Header Color = Orange**.
- `Combat|Weapons|Melee` has no explicit Hue rules.

A Melee member resolves Header Color to **Orange** from Weapons and Body Color to **Dark Gray** from Combat. Those values are inherited dynamically; Hue does not copy them into every child Category.

## A sensible workflow

Use a parent to establish a stable visual family. Add child overrides only where the user needs to distinguish a subset. If a child stops needing its exception, Clear that channel. This keeps the system easy to maintain.

## What counts as a Category?

These paths are the user-defined **My Blueprint** member categories for functions, macros, variables and applicable events. They are not the action-menu groupings shown when searching for nodes, and they are not arbitrary C++ class names.

---

For engine implementation, ownership and limits, [read Technical](TECHNICAL.md).
