# Hue Documentation Audit

**Baseline:** Hue 1.0.0. This initial pass follows the same documentation quality standard used for Focus.

## Coverage achieved

- Five user-centered categories with 19 feature topics and paired Design/Technical guides.
- Explicit source-backed coverage of scopes, four visual channels, nested inheritance, rename migration, global identities, selections, native node limitations and save ownership.
- Substantive category overviews and a roadmap without speculative release promises.
- Media placements with reader-facing captions, specific capture instructions and a machine-readable image/GIF registry.

## Remaining work

1. **Authentic imagery:** Replace placeholders with screenshots and recordings captured from Hue running in UE 5.8.
2. **Editor QA:** Execute the [Verification Matrix](VERIFICATION_MATRIX.md), especially category rename, global persistence, mixed selections and native specialized widgets.
3. **Browser QA:** Confirm responsive layout, accessibility, full-text search, image overflow and anchors in a real browser.
4. **Release-pinned source links:** When a stable Hue release tag is available, pin Technical source references to that tag rather than `main`.
5. **Support expansion:** Incorporate concrete user-reported edge cases when verified rather than inventing failures.

A C++ implementation audit documents intended/implemented behavior but is not a substitute for a passed test.
