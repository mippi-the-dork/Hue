# Supported Nodes and Native Controls — Technical

**Hue v1.0.0 · Unreal implementation reference**

## Two support checks

`FHueStyleResolver::CanStyleNode` reports whether Hue knows an appropriate construction path for the K2 class. `IsSupportedNode` confirms that a safe visual presentation has actually been established. Support must not be assumed merely because a node inherits from `UK2Node`.

## Registered factories

`FHueNodeFactory` uses `FEdGraphUtilities::RegisterVisualNodeFactory` and constructs Hue-specific graph widget subclasses for compatible families. `FHuePinFactory` participates in native-compatible pin rendering.

## Specialized native widgets

`FHueNativeNodeDecorator` attempts to bind only safe visual surfaces on native-specialized nodes. The decorator avoids title-color spill and preserves native controls, and can unmark provisional support when its attachment fails.

## Selection impact

`SHuePanel::GatherSelection` checks `IsSupportedNode` and counts unsupported nodes separately, so an incompatible item does not make a valid selection entirely unusable.

## Source

[HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp) · [HueNativeNodeDecorator.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNativeNodeDecorator.cpp) · [HuePinFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HuePinFactory.cpp).

---

For usage and examples, [read Design](DESIGN.md).
