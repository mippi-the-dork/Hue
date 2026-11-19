# Four Visual Channels — Technical

**Hue v1.0.0 · C++ source reference**

## Representation

[HueTypes.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Public/HueTypes.h) defines `EHueStyleChannel` and `FHueNodeStyleOverride`: four booleans paired with four `FLinearColor` fields. Each bool indicates whether the corresponding color is explicitly overridden. `IsEmpty()` is true when none of the four bits is active.

## Resolution

`FHueStyleResolver::ResolveColor` resolves an individual channel. A missing override is not equivalent to an explicit white color; it allows the resolver to continue through lower-precedence scopes.

## Slate presentation

Standard Hue `SGraphNode` subclasses and the native-node decorator bind compatible header/body and title/label surfaces to `ResolveColor`. `FHueNativeNodeDecorator` only decorates identifiable safe widgets; it avoids rewriting native editing controls. The pin factory supports compatible pin-text/exec-tint presentation while leaving native pin editing intact.

## Compact nodes

Hue presentation for certain operator and specialized widgets can rely on one visual fill. Code paths use the Header channel first and Body as a fallback when appropriate; do not document separate header/body parts where the actual native widget lacks them.

## Source references

[HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp) · [HueNativeNodeDecorator.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNativeNodeDecorator.cpp) · [HuePinFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HuePinFactory.cpp).

---

For normal user workflows, [read Design](DESIGN.md). Source-verified behavior is not a substitute for completed live editor QA.
