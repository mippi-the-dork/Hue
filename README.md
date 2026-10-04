# Hue 0.12.1 Prototype

Hue is an Unreal Engine Editor plugin for user-defined visual style overrides on Blueprint nodes.

Hue does not modify Unreal Engine source files and does not replace Blueprint node data classes. It changes only their Slate presentation.

## Product Goal

Every visually meaningful Blueprint node should support Hue wherever technically safe.

Hue preserves native node behavior first. When Unreal uses a specialized presentation, Hue either uses a dedicated compatible wrapper or lets Unreal build the native specialized widget and binds Hue only to safe visual layers.

## Visual Channels

Hue provides four independently overridable channels:

- Header Color
- Header Text Color
- Body Color
- Body Text Color

Body Text Color also controls compatible execution-pin triangles and pin labels. Editable value controls such as numeric fields, checkboxes, dropdowns, class pickers, and object pickers remain native.

## Precedence

```text
Instance
    >
Category
    >
Global
    >
Unreal Default
```

Category means the user-authored Category assigned to a Blueprint member in My Blueprint. It is not a node class or action-menu category.

## Broad K2 Coverage

Hue supports the normal K2 presentation plus broad compact-node coverage, including compatible Array, Map, Set, subsystem, conversion, autocast, macro, and function-library nodes.

Compact nodes use one central fill, so Hue maps Header Color to that fill and uses Body Color as its fallback.

## Specialized Coverage

Dedicated Hue-compatible presentation paths currently cover:

- Blueprint Events
- Blueprint Variable Get and Set
- promotable math operators
- Sequence and compatible Add Pin nodes
- Switch nodes
- Timeline
- Format Text
- collapsed graph / composite nodes

Hue 0.12.1 adds a guarded native-specialized bridge for node families whose Slate renderers are private to Unreal's GraphEditor module:

- Create Event / Create Delegate
- Spawn Actor
- Spawn Actor from Class
- Material Parameter Collection function nodes
- Make Struct presentations
- Copy nodes

For these families Unreal still constructs the native specialized widget. Hue then binds to the standard title, body, title-text, pin-label, and execution-pin visual layers it can identify safely. Native selectors, exposed-on-spawn controls, pin behavior, and other specialized interactions remain Unreal-owned.

## Right-Click Menu

Hue 0.12.1 registers its class-specific ToolMenu extensions even when Unreal has not registered the native menu yet. This is intentional: `UToolMenus::ExtendMenu()` supports late-created menus, which is important because Blueprint node context menus can be registered lazily.

Hue now refreshes its menu extensions after a Blueprint editor opens and when editor modules load. It also extends the registered K2 parent context menu when available.

Right-clicking a supported Blueprint node should expose the Hue submenu again.

## Documentation Nodes

Legacy Blueprint Documentation nodes are intentionally unsupported and completely ignored by Hue.

Runtime testing in Unreal Engine 5.8.3 showed the native Documentation node itself can crash inside Unreal's UDN documentation renderer even with Hue disabled. Hue therefore does not intercept, style, add menus to, or otherwise interact with `UEdGraphNode_Documentation`.

This is an intentional product exception rather than a pending Hue compatibility target.

## Remaining Compatibility Work

Known remaining areas include:

- Reroute/Knot nodes, where Hue's current header/body channels do not map meaningfully
- node-owned custom `CreateVisualWidget()` implementations that bypass registered visual node factories
- specialized third-party graph widgets outside the normal Blueprint K2 presentation path
- any native-specialized node whose internal widget rebuild proves to require a dedicated Hue wrapper rather than the 0.12.1 bridge

If a visually meaningful Blueprint node lacks Hue and can be supported without breaking native behavior, it remains a compatibility gap to investigate.

## Persistence

- Instance styles are stored with the Blueprint containing the node and keyed by `NodeGuid`.
- Category styles are stored with the Blueprint defining the categorized member.
- Global styles are project-shared in `Project Settings > Plugins > Hue`.
- Existing Global Function keys from earlier Hue versions remain compatible.

## Target

- Unreal Engine 5.8.0 - 5.8.3
- Windows 64-bit
- Editor Only
- Source prototype
