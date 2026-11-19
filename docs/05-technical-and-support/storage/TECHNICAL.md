# Data Storage and Reconciliation — Technical

**Hue v1.0.0 · Source-level reference**

## Serialized structures

`UHueBlueprintExtension` derives from `UBlueprintExtension` and contains `TMap<FGuid,FHueNodeStyleOverride> InstanceStyles` and `TMap<FString,FHueNodeStyleOverride> CategoryStyles`. This represents explicit overrides only; inherited values are derived on demand.

<figure class="doc-media" data-kind="image" data-media-id="hue-storage-technical"><img src="../../assets/media-placeholder.png" alt="Hue storage and style resolver technical data-flow diagram" loading="lazy"><figcaption>Hue separates serialized Blueprint style maps, project-wide configuration and computed effective colors.<span class="media-capture-instructions"><strong>Capture instructions · Image:</strong> Draw a data-flow diagram from selected UEdGraphNode/NodeGuid and My Blueprint Category to UHueBlueprintExtension maps, plus GetGlobalKey to UHueSettings GlobalFunctionStyles. Merge those inputs in HueStyleResolver before the final per-channel Slate tint.</span></figcaption></figure>

## Project settings

`UHueSettings` has `UCLASS(Config=Editor,DefaultConfig)` and `GlobalFunctionStyles` with `UPROPERTY(Config)`. The data is project/editor config, not a runtime Actor or Blueprint node data member.

## Migration of Category keys

`RefreshCategoryTrackingAndMigrate` identifies applicable changes following tracked Blueprint change/compile events and updates exact category style keys. It avoids treating ordinary member relocation as a wholesale Category rename.

## Transaction and invalidation

`SetColorOverrides`, `ClearColorOverrides` and `ClearAllOverrides` use `FScopedTransaction`, mark applicable owners as modified, save Global settings when necessary and invalidate presentation. Returning to an inherited source is a new resolution result, not a written snapshot of the parent's RGB value.

## Version boundary

The v1.0.0 implementation contains the above map names and storage model. A future format should introduce explicit migration rules rather than silently relying on current internals.

[HueBlueprintExtension.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueBlueprintExtension.h) · [HueSettings.h](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Public/HueSettings.h) · [HueStyleResolver.cpp](https://github.com/mippi-the-dork/Hue/blob/main/Source/Hue/Private/HueStyleResolver.cpp).

---

For practical use, [read Design](DESIGN.md).
