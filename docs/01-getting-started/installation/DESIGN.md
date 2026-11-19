# Installation and Compatibility — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Requirements

Hue v1.0.0 targets **Unreal Engine 5.8.0–5.8.3, Windows 64-bit** as documented by its current release. It is an editor-only plugin; no engine-source modification is needed.

## Install a packaged release

1. Close Unreal Editor.
2. Download a Hue ZIP matching the Unreal version/platform from [Hue Releases](https://github.com/mippi-the-dork/Hue/releases), or install the corresponding Fab package.
3. Extract the **Hue** folder beneath `YourProject/Plugins/`.
4. Confirm `YourProject/Plugins/Hue/Hue.uplugin` exists directly inside Hue—not a duplicated nested Hue folder.
5. Open the project, enable Hue in **Edit → Plugins** if needed, and restart when prompted.
6. Open a Blueprint Editor and find the **Hue** tab.

## Install from source

Clone the [Hue repository](https://github.com/mippi-the-dork/Hue) inside `YourProject/Plugins/Hue`, generate project files as needed, and build the Unreal **Development Editor / Win64** target with an appropriate C++ environment.

## Update a manual installation

Close Unreal, replace the entire existing Hue directory with the matching release, and reopen your project. Do not mix compiled binaries or source files from different plugin versions. Styles already saved in Blueprint assets or project configuration are separate from the plugin installation directory.

## If Hue does not appear

Verify platform and Unreal version, plugin activation, restart status, and that you're looking inside a **Blueprint Editor**, not the Level Editor Outliner. If the Hue panel was closed, restore its tab from the editor's tab controls.

---

For the underlying C++ implementation, see [Technical](TECHNICAL.md).
