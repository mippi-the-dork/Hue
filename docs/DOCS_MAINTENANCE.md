# Maintaining Hue Documentation

## Source of truth

Use the installed Hue plugin's current C++ source and its release README. Verify actual shipped behavior before updating a feature page; label unrun tests as unverified. The site is deployed from `main /docs` using GitHub Pages.

## Separate Design and Technical guides

- **Design:** workflow, user-facing behavior, examples, state meaning, limitations, expected outcomes and troubleshooting.
- **Technical:** relevant C++ types/functions, Unreal APIs, storage, target eligibility, transactions, ownership and compatibility boundaries.

Do not copy the same README section into both. Keep current source links appropriate for Hue v1.0.0 and update them or pin to a release tag if code changes later.

## Media replacement

Each placeholder figure contains a reader-facing caption plus visible **Capture instructions · Image/GIF** beneath it. Its stable ID, final asset path and capture details appear in [Media Inventory](MEDIA_INVENTORY.md) and [the media registry](assets/media-slots.json). Once a genuine screenshot or animation is captured, replace the placeholder source, keep captions accurate and remove or archive the capture-instruction line when the documentation is finalized.

## Maintenance checklist

Review affected Design and Technical files, update media references, validate relative links and navigation, run static HTML/JS checks, and perform real desktop/mobile browser verification. Verify actual engine behavior separately, and record passing or failing evidence in the [Verification Matrix](VERIFICATION_MATRIX.md).
