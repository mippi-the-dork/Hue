# Hue 0.12.1 Test Plan

## 1. Build and Open

1. Replace the previous Hue plugin with Hue 0.12.1.
2. Close Unreal Editor before building.
3. Build the project Editor target.
4. Launch Unreal Editor.
5. Open `HueTest` and open the Hue tab.

Expected: no crash and the Hue panel follows supported selected nodes.

## 2. Right-Click Hue Menu Regression

Right-click each exact node:

- Print String
- Branch
- Sequence
- Get CurrentHealth
- Array Add
- Map Add
- Get Game Instance Subsystem
- Event BeginPlay
- HueTestTimeline
- Create Event

Expected: every supported node shows the Hue submenu. The Hue panel and right-click menu edit the same stored styles.

## 3. Create Event

Place **Create Event** and connect its object/delegate context so Unreal exposes the native function selector.

Expected:

- Create Event is recognized by Hue
- Header Color works
- Header Text Color works on compatible title text
- Body Color works
- Body Text Color affects compatible pin labels and execution visuals where present
- the native function selector remains present and usable
- changing the selected function still reconstructs the node correctly
- undo/redo works
- compile, save/reopen, and copy/paste remain native

## 4. Spawn Actor Family

Place **Spawn Actor from Class**. Use an Actor class with at least one exposed-on-spawn variable.

Expected:

- Hue colors apply
- the Class picker remains native
- changing Class updates exposed-on-spawn pins
- exposed-on-spawn default controls remain native
- advanced pins still work
- compile, undo/redo, save/reopen, and copy/paste work

If available in the project, repeat with a node using `UK2Node_SpawnActor` rather than `UK2Node_SpawnActorFromClass`.

## 5. Make Struct

Create a Blueprint Structure named `HueTestStruct` with at least:

```text
Health : Float
Name   : String
Enabled: Boolean
```

Place **Make HueTestStruct**.

Expected:

- Hue colors apply
- struct member pins remain correct
- default-value widgets remain native
- Split/Recombine behavior on compatible struct pins remains normal
- changing the struct definition reconstructs the node correctly

Also place **Break HueTestStruct** and confirm its existing Hue path remains functional.

## 6. Material Parameter Collection

Create or use a Material Parameter Collection with a scalar parameter named `HueTestScalar`.

Place a Blueprint Material Parameter Collection operation such as **Set Scalar Parameter Value** using that collection.

Expected:

- Hue recognizes the specialized Material Parameter Collection call when Unreal uses that renderer
- collection/parameter selection UI remains native
- Hue visual overrides do not change runtime semantics

## 7. Copy Node

If the current Blueprint context exposes a `UK2Node_Copy` presentation, place that exact node and apply all four Hue channels.

Expected: the node remains behaviorally native and Hue changes only compatible visuals.

## 8. Compact Collections

Create:

```text
TestArray : Integer Array
TestMap   : String -> Integer Map
TestSet   : String Set
```

Test exact operations:

- Array Get
- Array Add
- Last Index
- Map Add
- Map Find
- Set Add
- Set Contains

Expected: existing 0.11 compact support remains intact.

## 9. Subsystem References

Place exact nodes where available:

- Get Game Instance Subsystem
- Get Local Player Subsystem
- Get World Subsystem
- Get Engine Subsystem
- Get Editor Subsystem in an editor-capable context

Expected: compact Hue styling and native subsystem class selection remain functional.

## 10. Existing Specialized Regression

Recheck:

- Add
- Multiply
- Get CurrentHealth
- Set CurrentHealth
- Switch on Int
- Sequence
- Format Text
- Event BeginPlay
- a Custom Event
- HueTestTimeline
- a collapsed graph

Expected: no regression in Hue styling or native interaction.

## 11. Documentation Node

Do not use the legacy Blueprint Documentation node for Hue testing.

Hue intentionally provides zero support for it and never intercepts it.

## 12. Scope Precedence

On a supported categorized Blueprint member node set different values for Global, Category, and Instance.

Expected:

```text
Instance > Category > Global > Unreal Default
```
