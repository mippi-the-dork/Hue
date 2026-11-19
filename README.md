# Hue

### Give your Blueprints a visual language.

**Persistent, flexible node styling for Unreal Engine.** Make the important parts of a graph recognizable at a glance—without changing how that Blueprint works.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.8.x-black?logo=unrealengine)
![Platform](https://img.shields.io/badge/Platform-Win64-blue)
![Plugin Type](https://img.shields.io/badge/Plugin-Editor%20Only-green)
![Version](https://img.shields.io/badge/Version-1.0.0-blue)
![License](https://img.shields.io/badge/License-MIT-green)

![Hue styling applied to an Unreal Engine Blueprint graph](Doc/Images/Hue-Hero.png)

> ### **Explore the complete Hue documentation**
>
> **[Open the Hue Documentation Website](https://mippi-the-dork.github.io/Hue/)**
>
> Everything you need to use Hue: getting started, practical workflows, every styling scope and visual channel, inheritance rules, supported nodes, saving, troubleshooting and technical implementation.
>
> **[Get Started](https://mippi-the-dork.github.io/Hue/?page=01-getting-started%2Ffirst-use%2FDESIGN.md)** · **[Visual Channels](https://mippi-the-dork.github.io/Hue/?page=02-styling-fundamentals%2Fchannels%2FDESIGN.md)** · **[Styling Scopes](https://mippi-the-dork.github.io/Hue/?page=02-styling-fundamentals%2Fscopes%2FDESIGN.md)** · **[Editing Workflows](https://mippi-the-dork.github.io/Hue/?page=04-editing-workflows%2FREADME.md)**

---

## Your Blueprints do more than execute logic. They communicate intent.

As Blueprints grow, everything starts competing for attention: functions, variables, events, validation, gameplay systems, and the occasional node that absolutely must not be overlooked.

Comments and clean layouts help, but they don't always tell you **what something means the moment you see it**.

Hue lets you establish a consistent visual language for your team. For example:

| Color | Meaning |
| --- | --- |
| 🟢 Green | Queries and read-only operations |
| 🔵 Blue | Movement |
| 🟠 Orange | Combat |
| 🟣 Purple | UI |
| 🔴 Red | Critical state changes |

*These are example conventions, not hardcoded Hue categories or required colors.*

A graph becomes easier to scan, a shared function becomes easier to recognize, and special-case logic can stand out without changing its behavior.

## Why use Hue?

**Color more than just a node's header.** Hue provides four independent channels: **Header Color, Header Text Color, Body Color and Body Text Color**. Change one without having to replace the others.

**Make your rules reusable.** Apply styling to a single **Instance**, to members of a user-defined **My Blueprint Category**, or to the same logical node identity **Globally** throughout the project.

**Build a hierarchy, not a maintenance burden.** Nested Categories inherit styling from their parents, channel by channel. Override only what needs to be different. Clear a rule and the next inherited color becomes visible.

**Style multiple nodes at once.** Select different compatible node types and edit them together. Hue reports mixed values and unsupported selections, and uses native Unreal **Undo/Redo** transactions.

**See where a color came from.** Hue reports whether a channel is explicitly overridden, inherited from a Category or parent Category, provided by a Global rule, or coming from Unreal's default.

**Work where you're already working.** Use the dedicated dockable **Hue panel** or a compatible node's **right-click → Hue** context menu. You don't need to change your Blueprint authoring workflow.

**Keep native controls intact.** Hue colors compatible Blueprint node presentations while preserving Unreal-owned inputs and interactions. It is an editor-only visual tool, not a gameplay framework.

## One system. Three levels of styling.

| Scope | Best for | Example |
| --- | --- | --- |
| **Instance** | One specific node occurrence | Highlight a critical `Apply Damage` call in a single graph |
| **Category** | Related Blueprint members | Give `Combat` functions and variables a shared header color |
| **Global** | The same logical node identity across the project | Make calls to a particular function consistently recognizable |

Hue resolves each channel independently:

```text
Instance
  ↓
Exact Category
  ↓
Nearest Parent Category → Higher Parents
  ↓
Global
  ↓
Unreal Default
```

That means one node can use a **Category header**, **Global body tint**, and **Instance text color** at the same time.

Your rules don't have to be duplicated across every graph. They work together, and you can still make exceptions when the context calls for it.

**Learn more:** [Style Precedence and Effective Sources](https://mippi-the-dork.github.io/Hue/?page=02-styling-fundamentals%2Fprecedence%2FDESIGN.md) · [Nested Category Inheritance](https://mippi-the-dork.github.io/Hue/?page=03-category-styling%2Fnested-inheritance%2FDESIGN.md)

## Built for real Blueprint workflows

Hue supports ordinary compatible K2 graph nodes and many specialized presentations, including functions, variables, events, operators, switches, sequences, timelines and composites where safe styling is possible.

A few examples of where it helps:

- **Gameplay systems:** Give Combat, Movement and UI recognizable color conventions that stay readable across larger graphs.
- **Shared functions:** Use Global styling to make repeat calls to the same function familiar wherever they appear.
- **Critical exceptions:** Override an individual node to emphasize a risky or high-impact operation without recoloring every use.
- **Refactoring:** Keep Category-based styles organized as nested Categories evolve; Hue tracks applicable Category renames.
- **Team readability:** Use consistent shared styles so a graph communicates structure to someone who didn't originally build it.

Hue stores **Instance and Category styles with their Blueprint assets** and **Global styles in project editor configuration**, making them suitable for normal team source-control workflows.

**Learn more:** [Hue Panel and Color Picker](https://mippi-the-dork.github.io/Hue/?page=04-editing-workflows%2Fhue-panel%2FDESIGN.md) · [Multi-Selection and Mixed Values](https://mippi-the-dork.github.io/Hue/?page=04-editing-workflows%2Fmulti-selection%2FDESIGN.md) · [Saving and Project Configuration](https://mippi-the-dork.github.io/Hue/?page=05-technical-reference%2Fstorage%2FDESIGN.md)

---

## Start styling

Hue **v1.0.0** targets **Unreal Engine 5.8.0–5.8.3 on Windows 64-bit**. It is an **Editor-only** plugin and does not require Unreal Engine source modifications.

You can install Hue from a compatible [GitHub Release](https://github.com/mippi-the-dork/Hue/releases), from Fab where available, or directly from source.

For detailed installation and update steps, follow the **[Installation and Compatibility Guide](https://mippi-the-dork.github.io/Hue/?page=01-getting-started%2Finstallation%2FDESIGN.md)**.

Once installed, open a Blueprint Editor, select a compatible node and open the **Hue** tab.

### Documentation shortcuts

| I want to… | Read |
| --- | --- |
| Style my first node | [Your First Styled Node](https://mippi-the-dork.github.io/Hue/?page=01-getting-started%2Ffirst-use%2FDESIGN.md) |
| Understand the four color channels | [Four Visual Channels](https://mippi-the-dork.github.io/Hue/?page=02-styling-fundamentals%2Fchannels%2FDESIGN.md) |
| Decide between Instance, Category and Global | [Styling Scopes](https://mippi-the-dork.github.io/Hue/?page=02-styling-fundamentals%2Fscopes%2FDESIGN.md) |
| Understand inherited colors | [Style Precedence](https://mippi-the-dork.github.io/Hue/?page=02-styling-fundamentals%2Fprecedence%2FDESIGN.md) |
| Work with nested Categories | [Category Styling](https://mippi-the-dork.github.io/Hue/?page=03-category-styling%2FREADME.md) |
| Batch-style a Blueprint selection | [Multi-Selection](https://mippi-the-dork.github.io/Hue/?page=04-editing-workflows%2Fmulti-selection%2FDESIGN.md) |
| Check which nodes can be styled | [Node Compatibility](https://mippi-the-dork.github.io/Hue/?page=04-editing-workflows%2Fnode-compatibility%2FDESIGN.md) |
| Understand how styles are saved | [Saving and Project Configuration](https://mippi-the-dork.github.io/Hue/?page=05-technical-reference%2Fstorage%2FDESIGN.md) |
| Fix an unexpected result | [Troubleshooting Hue](https://mippi-the-dork.github.io/Hue/?page=06-support%2Ftroubleshooting%2FDESIGN.md) |
| Explore the implementation | [Technical Reference](https://mippi-the-dork.github.io/Hue/?page=05-technical-reference%2FREADME.md) |

**[Browse the full documentation →](https://mippi-the-dork.github.io/Hue/)**

---

## What Hue doesn't do

Hue is a **Blueprint Editor presentation tool**, not a replacement for Blueprint functionality.

- It **does not** change node execution, data flow, compilation, runtime logic or packaged-game rendering.
- It **does not** add a gameplay or runtime dependency, and it requires **no Unreal Engine source changes**.
- It **does not** replace native editable node controls such as numeric fields, checkboxes, dropdowns or object/class pickers merely to recolor them.
- It **does not** guarantee styling for every third-party or specialized K2 presentation. Unsupported widgets retain their native appearance.
- **Reroute** and **Documentation** nodes are intentionally unsupported in v1.0.0.
- **Category** means a relevant user-defined **My Blueprint Category**, not an action-menu category or C++ node class.
- **Global** uses a logical Hue identity, not just a matching node title.
- It **does not** manage source-control checkouts, permissions or conflicts.

Hue favors preserving a node's native behavior over forcing a color change onto an incompatible interface.

For details, see [Supported and Unsupported Node Presentations](https://mippi-the-dork.github.io/Hue/?page=04-editing-workflows%2Fnode-compatibility%2FDESIGN.md).

## Reporting bugs

Found an issue? [**Open a bug report**](https://github.com/mippi-the-dork/Hue/issues).

Include the Hue version, exact Unreal Engine version, Windows version, installation method, affected Blueprint node type, styling scope and channel, whether single- or multi-selection was involved, and reproduction steps.

For visual issues, attach a **complete screenshot of the affected node** (not just the colored region), along with relevant Unreal Editor logs when available.

You can also consult the [Troubleshooting Guide](https://mippi-the-dork.github.io/Hue/?page=06-support%2Ftroubleshooting%2FDESIGN.md) before opening an issue.

## Feature requests

Have an idea for Hue? [**Suggest a feature**](https://github.com/mippi-the-dork/Hue/issues).

Tell us about the Blueprint readability or workflow problem you're trying to solve, not just the feature you'd like added. Hue's goal is to help communicate structure and intent while respecting Unreal's native editing experience.

For future directions and the current release baseline, see the [Roadmap](https://mippi-the-dork.github.io/Hue/?page=ROADMAP.md). Uncommitted ideas aren't promises of a future release.

## Contributions

Pull requests are welcome. For substantial additions, please open an [issue](https://github.com/mippi-the-dork/Hue/issues) first so we can discuss the intended behavior and integration path.

Contributions should preserve Hue's editor-only focus, native-safe styling, predictable persistence and existing Blueprint functionality. If a feature changes user-facing behavior, update the corresponding **Design and Technical documentation** alongside the code.

## License

Hue is distributed under the **MIT License**. See [LICENSE](LICENSE) for the full terms.

---

Hue is created by **[Mippi the Dork](https://github.com/mippi-the-dork)**.

**Make the Blueprint easier to read. Keep the Blueprint working exactly as it did before.**
