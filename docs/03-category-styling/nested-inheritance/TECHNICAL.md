# Nested Category Inheritance — Technical

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Category lookup algorithm

GetEffectiveCategoryColor examines the exact full category key, then successive parents separated by the vertical bar; the nearest applicable override wins. It also returns the source category key for UI reporting.

## Four independent lookups

Each EHueStyleChannel resolves on its own. An override on Header Color does not suppress inherited Body Text Color from a more distant parent.

## Runtime ownership

The rule remains on the defining Blueprint's CategoryStyles map. Descendants inherit by lookup, not by duplicated stored color records. No packaged-game visual data is needed.

---

## Source references

- [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp)
- [SHuePanel.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/SHuePanel.cpp)
- [HueMenu.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueMenu.cpp)
- [HueNodeFactory.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueNodeFactory.cpp)

For editor usage, [read Design](DESIGN.md). Source-reviewed claims are not equivalent to completed UE editor QA.
