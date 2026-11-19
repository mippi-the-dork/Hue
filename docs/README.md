# Hue Documentation

**Hue v1.0.0 · Unreal Engine 5.8.0–5.8.3 · Win64 · Editor only**

Hue adds persistent visual styling to compatible Unreal Engine **Blueprint nodes**. Build a readable visual language for functions, variables, events, macros and other supported graph elements without changing what the Blueprints do.

![A Blueprint graph organized using Hue](https://raw.githubusercontent.com/mippi-the-dork/Hue/main/Doc/Images/Hue-Hero.png)

*Hue adds four independent node-color channels and reusable Instance, Category and Global styling to compatible Blueprint graphs.*

## What can Hue do?

**Color separate parts of a node.** Configure Header Color, Header Text Color, Body Color and Body Text Color independently.

**Style one node or many.** Use Instance for one occurrence, Category for members of a user-defined My Blueprint category, or Global for a shared logical node identity.

**Make styles inherit naturally.** Exact and nested categories can supply per-channel colors, with the nearest matching source taking precedence over broader Global rules.

**Edit without changing behavior.** The Hue panel, native color picker, context-menu commands and supported Slate styling leave the graph's execution, data flow and native editable controls intact.

## Get started in three steps

1. [Install Hue](01-getting-started/installation/DESIGN.md) into a supported Unreal project.
2. [Style your first Blueprint node](01-getting-started/first-use/DESIGN.md) using the Hue panel's Instance Header Color.
3. [Learn inheritance](02-styling-fundamentals/precedence/DESIGN.md) and [Category styling](03-category-styling/category-identity/DESIGN.md) before authoring project-wide rules.

## Which guide do I read?

**Design** pages teach controls, step-by-step workflows, expected behavior and common pitfalls. **Technical** pages cover C++ ownership, graph factories, Slate decoration, native properties, persistence and integration boundaries.

## Explore the manual

| Section | Topics | Coverage |
| --- | ---: | --- |
| [01. Getting Started](01-getting-started/README.md) | 3 | Purpose, installation and first use |
| [02. Styling Fundamentals](02-styling-fundamentals/README.md) | 4 | Four channels, scope precedence, source reporting and clearing |
| [03. Category Styling](03-category-styling/README.md) | 3 | My Blueprint categories, nested inheritance and rename migration |
| [04. Editing Workflows](04-editing-workflows/README.md) | 4 | Hue panel, context menus, multi-selection and compatible nodes |
| [05. Technical Reference](05-technical-reference/README.md) | 4 | Editor integration, data ownership, identity resolution and Slate rendering |
| [06. Support and Troubleshooting](06-support/README.md) | 3 | Common questions, issue diagnosis and update guidance |

## Important limits

Hue works only in the Unreal Editor and only on compatible Blueprint node presentations. Documentation and reroute nodes are intentionally unsupported in v1.0.0. A visible action-menu category is not necessarily a My Blueprint Category; Global styling follows logical identity, not simply displayed titles.

Instance/Category styles are saved with Blueprint assets; Global styles are project editor configuration. Hue does not implement source control or modify packaged-game behavior.

## Roadmap and visual references

The [Roadmap](ROADMAP.md) describes the current v1.0.0 baseline and separates possible future directions from committed features. The [Media Inventory](MEDIA_INVENTORY.md) tracks the authentic screenshots and GIFs needed for the final documentation, with reader-facing captions and explicit capture instructions.

## Project and support

[Hue repository](https://github.com/mippi-the-dork/Hue) · [Releases](https://github.com/mippi-the-dork/Hue/releases) · [Report an issue](https://github.com/mippi-the-dork/Hue/issues)

**Verification:** The content is based on the current Hue source and README; independent UE/browser QA is separately tracked.
