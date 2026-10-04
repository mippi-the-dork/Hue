# Hue 0.13.0 Prototype

Hue is an Unreal Engine Editor plugin for user-defined visual style overrides on Blueprint nodes.

Hue does not modify Unreal Engine source files and does not replace Blueprint node data classes. It changes only their Slate presentation.

## Product Goal

Every visually meaningful Blueprint node should support Hue wherever technically safe.

Hue preserves native node behavior first. When Unreal uses a specialized presentation, Hue either uses a dedicated compatible wrapper or lets Unreal build the native widget and binds Hue only to visual layers it can identify safely.

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

Hue supports Unreal's normal K2 presentation plus compact nodes and Add Pin families.

The broad default path covers nodes such as:

- Branch and ordinary function calls
- Cast To nodes
- latent calls such as Delay
- async-task K2 nodes that use the normal K2 presentation
- Construct Object from Class
- Add Component by Class
- ForEachLoop and ForEachLoopWithBreak macro instances
- compatible conversion/autocast nodes

Compact nodes use one central fill, so Hue maps Header Color to that fill and uses Body Color as its fallback.

## Add Pin Families

Hue's Add Pin compatibility wrapper covers nodes implementing `IK2Node_AddPinInterface`, including:

- Sequence
- MultiGate
- Select
- Make Array
- Make Set
- Make Map
- compatible commutative/add-pin operators

Hue 0.13.0 brings this wrapper back in line with Unreal's native behavior by restoring the Add Pin transaction and native visibility rule. Adding a pin is now a normal Undo/Redo operation, and the button collapses when the node reports that another pin cannot be added.

## Specialized Coverage

Dedicated Hue-compatible presentation paths cover:

- Blueprint Events
- Blueprint Variable Get and Set
- promotable math operators
- Sequence and compatible Add Pin nodes
- Switch nodes
- Timeline
- Format Text
- collapsed graph / composite nodes

Hue also has a guarded native-specialized bridge for node families whose Slate renderers are private to Unreal's GraphEditor module:

- Create Event / Create Delegate
- Spawn Actor
- Spawn Actor from Class
- Material Parameter Collection function nodes
- Make Struct presentations
- Copy nodes

For these families Unreal still constructs the native specialized widget. Hue then binds to the standard title, body, title-text, pin-label, and execution-pin visual layers it can identify safely. Native selectors, exposed-on-spawn controls, pin behavior, and other specialized interactions remain Unreal-owned.

## Node-Owned Visual Widgets

Some K2 nodes can bypass every registered graph-node factory by returning their own Slate widget from `CreateVisualWidget()`.

Create Widget remains the known engine case Hue explicitly hooks during pin construction so its execution pins can participate in Hue. Hue 0.13.0 also adds a general compatibility fallback for any displayed K2 node that bypassed Hue's factory:

1. Hue reaches the native displayed node only after Unreal has created it.
2. Hue scans without changing the widget.
3. Support is accepted only if both a compatible standard header and body surface are present.
4. Only then does Hue attach its color attributes and mark the live node supported.
5. If those surfaces are not present, the node remains untouched and Hue does not offer controls for it.

This closes the false-positive case where a node could appear supported even though Hue had no usable visual surface to style.

## Confirmed Visual Support

Hue separates potential K2 capability from confirmed visual support.

The Hue panel, right-click menu, and style mutation commands only treat a live node as supported after Hue has established one of these paths:

- a Hue-owned compatible wrapper,
- a validated native-specialized bridge,
- or a validated node-owned native widget fallback.

Known native-specialized bridges are now provisional during construction. If the finished native widget cannot be decorated, Hue retracts the support claim and its execution-pin widget falls back to Unreal's native color.

## Right-Click Menu

Hue registers class-specific ToolMenu extensions even when Unreal has not registered the native menu yet. This is intentional because Blueprint node context menus can be registered lazily.

Hue refreshes its menu extensions after a Blueprint editor opens and when editor modules load. It also extends the K2 parent context menu when available.

Right-clicking a supported Blueprint node should expose the Hue submenu.

## Documentation and Reroute Nodes

Legacy Blueprint Documentation nodes are intentionally unsupported and completely ignored by Hue.

Reroute/Knot nodes are also intentionally excluded because Hue's current header/body channels do not map meaningfully to their control-point presentation.

## Compatibility Sweep

Hue 0.13.0 adds a formal compatibility matrix and regression plan covering:

- object construction
- Add Pin families
- Switch and control-flow nodes
- macros and latent/async nodes
- structs
- containers
- Cast nodes
- selection/hover/error/breakpoint visuals
- reconstruction, duplication, copy/paste, save/reopen, and Blueprint recompilation

See `Doc/Compatibility-Matrix.md` and `Doc/Prototype-Test-Plan.md`.

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
