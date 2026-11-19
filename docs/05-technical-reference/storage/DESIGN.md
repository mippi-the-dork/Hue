# Saving and Project Configuration — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## What saves where

| Scope | Storage | What to save/share |
| --- | --- | --- |
| Instance | Node-specific style in owning Blueprint asset | Save the Blueprint; commit its asset |
| Category | Category style on the Blueprint defining its members | Save that defining Blueprint; commit its asset |
| Global | Hue project-level Editor configuration | Share that config under your project workflow |

Changing Hue colors doesn't change what the Blueprint does at runtime.

## Team use

A teammate needs the relevant Blueprint asset for Instance/Category styles and the project configuration for Global styles. Hue does not handle checkouts, Perforce conflicts or Git merges; those remain your source-control system's responsibility.

## Update the plugin safely

Hue's saved styles aren't stored inside the installed plugin directory, but replacing a plugin while the editor is running is still a bad idea. Close Unreal before replacing Hue binaries and source.

## Before submitting

Save changed Blueprints and check whether the intended project Hue settings are shared. A local-only editor config might not automatically be tracked by your source-control policy.

## Undo versus saved state

Unreal Undo can revert a Hue action in the current session. Save the relevant Blueprint or configuration afterward to persist the intended new state.

---

For native implementation details, [read Technical](TECHNICAL.md).
