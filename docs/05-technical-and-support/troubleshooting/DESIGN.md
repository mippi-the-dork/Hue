# Troubleshooting and FAQ — Design

**Hue v1.0.0 · User guide**

## Hue tab is missing

Confirm the Hue plugin is enabled under **Edit → Plugins**, your UE installation is a supported 5.8.x Win64 Editor, and you've reopened the Blueprint Editor after activation. Search the Blueprint editor's available tabs for Hue; it does not appear as an Actor or ordinary Content Browser asset.

<figure class="doc-media" data-kind="image" data-media-id="hue-troubleshooting-panel"><img src="../../assets/media-placeholder.png" alt="Hue panel unsupported and unavailable scope diagnostics" loading="lazy"><figcaption>Hue identifies unavailable scopes and mixed-value selections without modifying unsupported nodes.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Capture the Hue panel with a selection containing at least one unsupported node and one supported node. Show the supported/unsupported selection counts and an unavailable Category or Global scope, keeping the selected graph nodes visible.</span></figcaption></figure>

## Selected node is unsupported

Hue only offers styling for presentations it can safely control. Reroute nodes, Documentation nodes and unsupported custom graph widgets are intentionally left native. Try a normal supported function-call or variable node to distinguish a compatibility limitation from a plugin load problem.

## Category scope is unavailable

Make sure the node corresponds to a Blueprint-defined function, variable, macro or event with an applicable **My Blueprint** Category. A generic action menu category is not a My Blueprint member Category.

## Clear didn't restore Unreal's original color

The node may still inherit a style from its Category parent or Global identity. Check the source label beside the channel. Clear a higher-priority explicit rule to reveal the next source; this may require clearing more than one scope.

## Two nodes look the same but show Multiple Values

Hue tracks whether each target explicitly owns a value as well as its visible color. One node may explicitly specify the same RGB as another node inherits, so their underlying states differ.

## Child category changed when parent was edited

Expected if that child lacks its own override for the channel. Set a child override for the exceptional value or clear it to rejoin parent inheritance.

## Moving one member didn't move its Category style

Expected. Changing one member's Category does not rename the old shared Category. Hue distinguishes these operations.

## Get and Set variable nodes changed together

Expected for the same Global Variable identity, since both represent the same underlying Blueprint member.

## Reporting a reproducible issue

Include UE/Hue version, exact `UK2Node` type, graph/Blueprint type, chosen scope and channel, support/selection state, original and resulting color, steps, screenshots and Output Log. Check the [Verification Matrix](../../VERIFICATION_MATRIX.md) before assuming a path has passed real-editor testing.

---

For code and implementation, [read Technical](TECHNICAL.md).
