# Supported and Unsupported Node Presentations — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Two levels of eligibility

FHueStyleResolver::CanStyleNode indicates a supported potential node presentation. IsSupportedNode requires a confirmed live styling path through MarkVisualSupport/UnmarkVisualSupport.

## Factories and native decoration

FHueNodeFactory creates specialized SHueGraphNode variants. Some node families use a native self-visual bridge; FHuePinFactory and FHueNativeNodeDecorator attach styling to safe Slate surfaces without replacing node logic.

## Safety behavior

When Hue cannot identify a compatible native widget, it leaves the node alone. Supported K2 class identity is not sufficient if the actual Slate presentation cannot be colored safely.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueMenu.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueMenu.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)

For editor usage, [read Design](DESIGN.md). Source-reviewed claims are not equivalent to completed UE editor QA.
