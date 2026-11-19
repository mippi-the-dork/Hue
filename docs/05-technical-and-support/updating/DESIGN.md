# Updating Hue and Getting Support — Design

**Hue v1.0.0 · User guide**

## Updating from a release

1. Close the Unreal Editor.
2. Obtain the matching Hue release ZIP for your Unreal Engine 5.8.x and Win64 setup.
3. Replace the **entire** existing `YourProject/Plugins/Hue` folder.
4. Reopen the project and confirm the v1.0.0 (or newer release) version under Plugins.
5. Open a Blueprint and verify a known styled node, Hue panel, and Global settings.

<figure class="doc-media" data-kind="image" data-media-id="hue-release-update"><img src="../../assets/media-placeholder.png" alt="Hue plugin installation folder prepared for release update" loading="lazy"><figcaption>Replacing the complete Hue plugin folder keeps source and precompiled release files from becoming mixed.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture Windows File Explorer showing the Hue folder immediately inside a project's Plugins directory, with Hue.uplugin and Source/Resources/Config at the right level. Include a small label indicating the replacement is performed with Unreal Editor closed.</span></figcaption></figure>

## Updating a source checkout

Pull the intended commit/release tag in your Hue Git checkout, regenerate Visual Studio project files if required, build the Editor target, and restart Unreal.

## Supported release

The documented baseline is **Hue v1.0.0**, **Unreal Engine 5.8.0–5.8.3**, **Win64**, Editor only. A `Hue.uplugin` descriptor targeting 5.8.0 is not evidence of support for arbitrary future engines.

## What happens to styles during updating?

Instance and Category styles are Blueprint asset data. Global styles are in the project's editor configuration. They are not stored in the installed plugin folder; you should not delete Blueprint assets or shared config while replacing the plugin itself.

## Reporting problems

Use [Hue GitHub Issues](https://github.com/mippi-the-dork/Hue/issues). Include installed plugin version, exact UE version, node family, visual channel, Instance/Category/Global scope, whether multi-selection was active, steps to reproduce and a full node screenshot or short recording.

## Feature ideas

Explain the actual Blueprint readability or graph-authoring problem you want solved. Hue's guiding constraint is preserving native node functionality and compiled behavior while improving visual organization.

---

For code and implementation, [read Technical](TECHNICAL.md).
