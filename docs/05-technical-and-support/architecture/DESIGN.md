# Editor Architecture — Design

**Hue v1.0.0 · User guide**

## How Hue changes the graph without changing the Blueprint

Hue integrates into native Unreal Blueprint editing. Its panel and context menus author style rules; a resolver computes the effective colors; compatible graph node and pin presentations display those colors. The Blueprint's execution/data flow remains the same.

<figure class="doc-media" data-kind="image" data-media-id="hue-architecture-diagram"><img src="../../assets/media-placeholder.png" alt="Hue Editor integration architecture diagram" loading="lazy"><figcaption>Hue's editor-only architecture separates style authoring, rule resolution and native-safe Blueprint node presentation.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Create a technical diagram with HueModule, HuePanel and HueMenu feeding HueStyleResolver. Show Instance/Category data in Blueprint Extension, Global data in HueSettings, then arrows to HueNodeFactory/PinFactory and NativeNodeDecorator rendering Slate nodes. Mark Unreal Blueprint execution as unchanged.</span></figcaption></figure>

## Why native-safe rendering matters

Blueprint nodes have many specialized widgets, and a naive recoloring approach could change widget behavior or readability. Hue uses custom presentation only for known compatible paths and decorates selected native widgets when safe. Native numeric entry, object/class selectors and other editable controls stay Unreal-owned.

## What happens with an unusual node

The node must have a valid logical style target **and** a supported visual presentation. Hue can recognize some nodes broadly without claiming the actual displayed native widget is safe to modify. Unsupported or unconfirmed presentations remain unchanged.

## Performance design

Style lookup is independent of expensive category rename detection. The latter follows Blueprint change/compile events rather than repeated work in every Slate paint. Color resolution can read cached/current style data without rebuilding Blueprint member graphs each frame.

## Editor-only boundary

Hue is not a gameplay system, rendering feature for packaged games or engine patch. It supplies editor presentation for K2 graphs.

---

For the Unreal implementation, [read Technical](TECHNICAL.md).
