# What Hue Does — Technical

**Hue v1.0.0 · C++ source reference**

## Editor module

[Hue.uplugin](https://github.com/mippi-the-dork/Hue/blob/main/Hue.uplugin) declares a `Hue` module with `Type: Editor`, `LoadingPhase: PostEngineInit`, and Win64 support. It contains no runtime module or plugin Content folder.

## Core responsibilities

`FHueModule` registers graph node and pin factories, Hue's Blueprint tab factory, contextual ToolMenus, and Blueprint change/compile tracking. `FHueStyleResolver` determines whether a node is supported and resolves four independent color channels. `SHuePanel` edits styles through native Slate.

## Identity and presentation

Supported K2 node types have two separate concerns: a **logical style identity** and a **confirmed visual presentation**. `CanStyleNode` is a broad construction-time check; `IsSupportedNode` becomes true only after Hue has actually established its safe rendering path.

## No behavior changes

Hue stores visual overrides and applies colors to Slate surfaces. It does not alter executable pins, function invocation, Blueprint compilation or runtime data flow. Native input widgets remain Unreal-owned.

## Source references

[HueModule.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueModule.cpp) · [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp) · [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp).

---

For normal user workflows, [read Design](DESIGN.md). Source-verified behavior is not a substitute for completed live editor QA.
