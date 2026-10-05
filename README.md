# Hue

**Persistent visual styling for Unreal Engine Blueprint nodes.**

Hue adds editor-only color controls to compatible Blueprint nodes without changing Blueprint behavior. Style one node, an entire Blueprint Category, or the same logical node identity project-wide while preserving Unreal's native node interactions.

**Target:** Unreal Engine 5.8.x on Windows 64-bit. Primary validation is Unreal Engine 5.8.3.

Hue is an Editor-only plugin. It requires no engine source modifications and adds no runtime system to packaged games.

## Features

### Four visual channels

Hue exposes four independent presentation channels:

- **Header Color**
- **Header Text Color**
- **Body Color**
- **Body Text Color**

Body Text Color also affects compatible execution-pin triangles and pin labels. Editable value controls such as numeric fields, checkboxes, dropdowns, class pickers, and object pickers remain Unreal-owned.

### Three styling scopes

Hue supports three override scopes:

1. **Instance** - one specific node instance.
2. **Category** - matching Blueprint members in a user-authored My Blueprint Category.
3. **Global** - the same Hue identity everywhere the plugin can identify it safely.

Resolution order:

```text
Instance
> Exact Category
> Nearest Parent Category
> Higher Parent Categories
> Global
> Unreal Default
```

Each visual channel resolves independently. A node can inherit its Header Color from a parent Category while overriding only its Body Color locally.

### Nested Category inheritance

Categories use Unreal's `|` hierarchy syntax.

Example:

```text
Attack
Attack|Weapons
Attack|Weapons|Melee
```

If `Attack` owns a Body Color and its descendants do not, they inherit it. A closer child rule wins only for the channels it explicitly overrides.

Parent values are not copied into child data. Changing or clearing a parent rule updates descendants dynamically.

### Category rename persistence

Hue tracks category structure while a Blueprint is open so renaming a styled My Blueprint Category keeps its Hue settings.

Renaming:

```text
Attack -> Combat
```

can also migrate styled descendants such as:

```text
Attack|Weapons -> Combat|Weapons
```

Moving one member to another Category is not treated as a Category rename.

### Multi-selection and batch editing

Select unrelated compatible Blueprint nodes and edit Hue properties together.

Batch editing supports:

- mixed values
- Instance edits across every compatible selected node
- Category edits across unique represented Categories
- Global edits across unique represented Hue identities
- unsupported nodes remaining selected without being modified
- one Unreal transaction per batch operation
- one Undo or Redo step for the full batch

The Hue panel reports `Override`, `Inherited`, `Multiple Values`, or `Unavailable` as appropriate.

### Effective source reporting

For a single selected node, Hue explains where the current visible value comes from. Depending on the channel, the panel can report sources such as:

```text
Override
Category: Attack|Weapons
Parent: Attack
Global Function
Global Variable
Global Event
Unreal Default
```

This makes the active precedence path visible instead of requiring the user to infer it.

### Right-click workflow

Compatible Blueprint nodes expose a **Hue** submenu in their context menu.

When the right-clicked node belongs to a compatible multi-selection, Hue operations can apply to the selected set rather than only the clicked node.

### Native-safe presentation

Hue preserves native Blueprint behavior first.

Standard K2 nodes use Hue-compatible wrappers around Unreal's normal presentation. Specialized nodes continue using Unreal's native widget when replacing that widget could break selectors, dynamic pins, exposed-on-spawn controls, or other node-specific behavior. Hue attaches only to visual surfaces it can identify safely.

If Hue cannot safely style a node presentation, it leaves that presentation untouched instead of exposing controls that do nothing.

## Supported node coverage

Hue covers the normal K2 presentation and a broad set of specialized families, including:

- standard function calls
- pure and impure calls
- Blueprint Events
- Variable Get and Set
- Cast nodes
- latent calls such as Delay
- compatible async-task nodes
- compact operators and conversion nodes
- Sequence and compatible Add Pin nodes
- Select
- MultiGate
- Make Array, Make Set, and Make Map
- Switch nodes
- Timeline
- Format Text
- collapsed graph and composite nodes
- Construct Object from Class
- Add Component by Class
- Create Widget
- Spawn Actor and Spawn Actor from Class
- Create Event and Create Delegate
- Material Parameter Collection function nodes
- Make Struct presentations
- Copy nodes when exposed by the current Blueprint context

Compatibility is presentation-dependent. Third-party or future engine nodes are styled only when Hue can confirm a safe visual path.

## Intentionally unsupported

### Reroute Node

Reroute nodes do not expose the visual surfaces Hue is designed to style, so Hue does not offer controls for them.

### Documentation Node

The Blueprint Documentation node remains intentionally unsupported. Hue leaves its specialized presentation untouched.

### Unknown custom widgets

A custom K2 node may work automatically if its displayed widget exposes compatible standard graph surfaces. Otherwise Hue leaves it unchanged.

## Using Hue

1. Open a Blueprint Editor.
2. Open the **Hue** tab from the Blueprint Editor's tab/view menu if it is not already visible.
3. Select one or more compatible Blueprint nodes.
4. Choose the **Instance**, **Category**, or **Global** scope.
5. Set any combination of Header Color, Header Text Color, Body Color, and Body Text Color.
6. Use **Clear** for one channel or **Clear All Overrides** for the current scope to reveal lower-precedence values again.

You can perform the same core styling operations through the node right-click **Hue** submenu.

## Installation

### Packaged plugin

1. Close Unreal Editor.
2. Extract the `Hue` folder into your project's `Plugins` folder.
3. Confirm the descriptor is at `YourProject/Plugins/Hue/Hue.uplugin`.
4. Open the project and enable **Hue** under **Edit > Plugins**.
5. Restart Unreal Editor if prompted.

Use a package built for your Unreal Engine version and platform. If Unreal reports incompatible binaries, use a matching package or build Hue from source.

### Source installation

1. Place the `Hue` folder in your C++ project's `Plugins` folder.
2. Close Unreal Editor.
3. Generate Visual Studio project files if needed for the initial installation.
4. Build your project's **Development Editor / Win64** target.
5. Open the project, enable **Hue**, and restart if prompted.

A source build requires a working Unreal Engine C++ toolchain.

## Compatibility

| | |
| --- | --- |
| **Hue Version** | 0.19.0 Release Candidate |
| **Unreal Engine** | 5.8.x |
| **Primary Validation Version** | 5.8.3 |
| **Platform** | Windows 64-bit |
| **Plugin Type** | Editor Only |
| **Runtime Dependency** | None |
| **Packaged Game Impact** | None |

Hue's descriptor uses Unreal Engine 5.8.0 as its engine-version baseline. Compatibility with engine versions or platforms outside the listed target should not be assumed unless explicitly validated.

## How Hue works

Hue stores presentation metadata separately from Blueprint execution behavior.

At a high level:

1. Hue identifies the selected Blueprint node and the applicable styling scopes.
2. Instance, Category, nested parent Category, and Global rules are resolved in one precedence path.
3. Hue supplies those visual values through a compatible Slate presentation path.
4. Native node controls and Blueprint logic remain Unreal-owned.
5. Unsupported presentations are left untouched.

Category rename tracking runs when the Blueprint reports structural changes rather than during normal Slate paint. Custom-widget compatibility failures are cached per displayed Slate widget so large graphs and multiple Blueprint Editor panels do not repeatedly rescan the same rejected presentation.

## What Hue does not do

Hue is a Blueprint Editor presentation tool. It does not:

- change Blueprint execution
- add runtime gameplay systems
- modify packaged-game rendering
- modify Unreal Engine source
- force unsupported nodes through a generic replacement widget
- recolor editable value controls that belong to Unreal's native node UI
- treat action-menu categories or C++ node classes as My Blueprint Categories

## Release Candidate status

0.19.0 is the feature-frozen release candidate for Hue 1.0.0.

No new features are planned between this release candidate and 1.0.0 unless validation exposes a release-blocking issue. The remaining work is compile validation, regression testing, documentation verification, and final release packaging.

The full validation matrix and test plan are included in `Doc/`.
