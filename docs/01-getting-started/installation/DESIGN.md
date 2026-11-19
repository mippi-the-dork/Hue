# Installation and Compatibility — Design

**Hue v1.0.0 · User guide**

## Recommended installation

For most users, install Hue through Fab or a compatible GitHub Release. Hue v1.0.0 targets **Unreal Engine 5.8.0–5.8.3, Windows 64-bit**, with UE 5.8.3 identified as the primary validated engine version in the release README.

### From Fab

Add Hue to your library, install it for the matching Unreal version through Epic Games Launcher, then enable it under **Edit → Plugins** if required. Restart the Editor if prompted.

### From GitHub Releases

1. Download a release ZIP built for your engine and Win64 from [Hue Releases](https://github.com/mippi-the-dork/Hue/releases). GitHub's automatically generated Source ZIP is not a compiled plugin release.
2. Close the Unreal Editor.
3. Extract the complete `Hue` folder into `YourProject/Plugins/` so `Hue.uplugin` is at `YourProject/Plugins/Hue/Hue.uplugin`.
4. Reopen the project, enable Hue in Plugins and restart if requested.

<figure class="doc-media" data-kind="image" data-media-id="install-folder"><img src="../../assets/media-placeholder.png" alt="Hue plugin installed in an Unreal project Plugins folder" loading="lazy"><figcaption>Hue can be installed directly in a project's Plugins directory without changing Unreal Engine source.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture Windows File Explorer showing YourProject/Plugins/Hue in the address bar with Hue.uplugin, Source, Resources and Config visible together. Do not show a doubly nested Hue/Hue folder.</span></figcaption></figure>

### From source

Clone [the Hue repository](https://github.com/mippi-the-dork/Hue) to `YourProject/Plugins/Hue`, generate Visual Studio project files if required, build the **Development Editor / Win64** target, then launch your project.

## Updating an installation

Close Unreal first. Replace the entire prior plugin folder with the new matching release, or pull and rebuild if you installed from Git source. Your Blueprint assets and project editor settings are separate from the plugin installation directory.

## After installation

Open a Blueprint Editor, select a normal compatible K2 node, and look for the **Hue** tab. If it is absent, check plugin activation, exact UE version, build logs and available Blueprint Editor tabs.

---

For Unreal implementation, data ownership and boundaries, [read Technical](TECHNICAL.md).
