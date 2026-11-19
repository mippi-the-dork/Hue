# Updating, Reporting Bugs and Support — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Updating Hue

<!-- Media target: IMAGE | upgrade-layout | docs/assets/media/upgrade-layout.png -->
<figure class="doc-media" data-kind="image" data-media-id="upgrade-layout">
  <img src="../../assets/media-placeholder.png" alt="Updating a manual Hue installation means replacing the complete plugin directory with a compatible release." loading="lazy">
  <figcaption>Updating a manual Hue installation means replacing the complete plugin directory with a compatible release.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture Hue.uplugin directly beneath Plugins/Hue in File Explorer, with Source, Resources and Config shown; optionally label the matching release version.</span></figcaption>
</figure>


For manual release installations, close Unreal Editor, replace the entire Hue plugin directory, then reopen the project. For source installations, pull the chosen version and rebuild the project's Editor target as needed.

## Current supported release

Hue **v1.0.0**, targeted to **UE 5.8.0–5.8.3, Win64**, editor only. The plugin doesn't modify engine source or packaged-game behavior. Check later releases for updated compatibility.

## Which data should remain?

Styles stored on Blueprint assets and in project Hue config are separate from the plugin directory. Do not delete Blueprint assets or project config to upgrade Hue.

## Reporting a problem

Use [Hue Issues](https://github.com/mippi-the-dork/Hue/issues). Include the Hue/UE versions, installation route, precise node type, applicable scope, visual channel, selection count, reproduction steps and an un-cropped node screenshot where a rendering issue is involved.

## Contributions

A useful feature request describes the Blueprint readability problem first, then the intended workflow. For large structural changes, discuss the intended supported node families and performance implications before implementation.

## License

Hue is published under the MIT license. The repository LICENSE contains the legal terms.

---

For native implementation details, [read Technical](TECHNICAL.md).
