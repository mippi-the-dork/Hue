# Hue Roadmap

**Current shipped release:** Hue v1.0.0 · UE 5.8.0–5.8.3 · Win64 · Editor only.

## Release history and current state

<!-- Media target: IMAGE | roadmap-release-status | docs/assets/media/roadmap-release-status.png -->
<figure class="doc-media" data-kind="image" data-media-id="roadmap-release-status">
  <img src="assets/media-placeholder.png" alt="Hue v1.0.0 establishes the shipped visual styling foundation; later additions remain uncommitted." loading="lazy">
  <figcaption>Hue v1.0.0 establishes the shipped visual styling foundation; later additions remain uncommitted.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Create a clear release timeline with v1.0.0 Released and a separate Future / Not Committed area. Do not invent version numbers, feature lists or dates.</span></figcaption>
</figure>


| Release | Status | Documented functionality |
| --- | --- | --- |
| **v1.0.0 — Visual styling foundation** | Released | Four independent visual channels; Instance, Category and Global scopes; per-channel inheritance; nested categories; category rename tracking; mixed selection; right-click and Hue panel editing; native-safe presentation |

The current repository/README does not establish a different historical major-version sequence. A later version should be added to this table only when a release actually exists.

## Future direction — not committed

Hue's current purpose suggests potential future research into further **native-safe specialized Blueprint node coverage**, workflow polish, more helpful availability diagnostics and stronger regression automation. These are **possibilities to evaluate**, not announced features or a specific v2.0 commitment.

Any future feature proposal should preserve Hue's main constraint: recoloring a node must not change its execution, editable native controls, or packaged-game behavior.

## Release quality requirements

For a new supported node family, require a verified native Slate styling path, a supported/unsupported boundary, Undo/Redo testing, compatibility verification on the target UE version, and updated Design/Technical documentation.

For changes to the scope resolver, verify shared logical identity, nested category inheritance, rename migration, mixed state and Blueprint/project-config persistence. Document failures as well as successes.

## Contributing ideas

Use [Hue Issues](https://github.com/mippi-the-dork/Hue/issues) to describe the readability problem and the intended editing workflow. Nothing on this page should imply that speculative concepts are already available.
