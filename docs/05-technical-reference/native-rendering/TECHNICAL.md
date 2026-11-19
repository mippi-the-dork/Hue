# Native Slate Rendering and Compatibility — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Slate implementations

<!-- Media target: IMAGE | slate-surface-tech | docs/assets/media/slate-surface-tech.png -->
<figure class="doc-media" data-kind="image" data-media-id="slate-surface-tech">
  <img src="../../assets/media-placeholder.png" alt="Native-safe coloring affects selected Slate surfaces without replacing unrelated controls." loading="lazy">
  <figcaption>Native-safe coloring affects selected Slate surfaces without replacing unrelated controls.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Diagram HueNodeFactory, HuePinFactory and HueNativeNodeDecorator attaching color attributes to valid SGraphNode/SGraphPin surfaces, leaving editable controls intact.</span></figcaption>
</figure>


SHueGraphNodeCallFunction, SHueGraphNodeK2Var, SHueGraphNodeK2Event, SHueGraphNodeK2Switch, SHueGraphNodeK2Timeline, SHueGraphNodeK2Operator and other specialized SHueGraphNode classes customize eligible surfaces.

## Native decoration

HueNativeNodeDecorator::Apply binds color attributes to selected safe native child widgets instead of wholesale replacing the underlying graph node. The pin factory can supply SHueGraphPinExec for compatible execution-pin visuals.

## Confirmed support

HueStyleResolver::CanStyleNode is permissive capability detection; IsSupportedNode requires confirmed decoration. UnmarkVisualSupport removes provisional state when a bridge cannot attach.

## Engine semantics

The style hooks preserve editable native Slate widget behavior and normal graph lifecycle. Faded/deactivated node visuals retain Unreal's subdued treatment rather than ignoring enabled state.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)
- [HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h)

For user-facing behavior, [read Design](DESIGN.md). Source inspection alone does not establish completed live Unreal Editor testing.
