# Hue Documentation Quality Audit

**Baseline:** Hue v1.0.0 · UE 5.8.0–5.8.3 · Win64. This is a **source-based editorial audit**, not a completed hands-on editor or browser test report.

## Coverage

- **Six distinct sections, 21 feature topics and 42 Design/Technical pages.** The section hierarchy follows Hue's actual four color channels, three scopes, Category inheritance, editing workflows and native rendering implementation.
- Source-grounded documentation of Global logical identity, My Blueprint Category scope, rename/move distinctions, color precedence, clear/clear-all behavior, mixed selections and supported native widgets.
- A substantive homepage and section overviews, with search and direct Design/Technical switching.
- **43 typed image/GIF slots** across 36 pages, with visible reader captions and exact capture instructions; real hero and plugin icon reused.
- A version-honest Roadmap and an explicit [Verification Matrix](VERIFICATION_MATRIX.md) that marks tests **Not run** until evidence exists.

## Remaining high-priority work

| Priority | Gap | Action |
| --- | --- | --- |
| P1 | Visuals are placeholders | Capture authentic Hue editor screenshots and short recordings using the [Media Inventory](MEDIA_INVENTORY.md) |
| P1 | Editor behavior is source-reviewed but not run here | Execute the [Verification Matrix](VERIFICATION_MATRIX.md), especially inheritance, category renames, mixed selections, save/reopen and specialized native node families |
| P1 | Site has only static validation | Test search, links, narrow screens, image containment, keyboard navigation and text contrast in an actual browser |
| P2 | Technical source links track `main` | Pin to a stable release tag or source commit when available and maintain docs with each Hue update |
| P2 | Rare edge cases need evidence | Incorporate verified bug reports instead of inventing unsupported fixes or claiming undocumented compatibility |

## Quality standard for future pages

**Design** must answer what a user needs, how to do it, what the result should look like, which scope owns it, and how to undo or troubleshoot it. **Technical** must identify actual Hue C++ owners, Slate/Blueprint integration, storage, precedence and unsupported paths.

A passing JavaScript parser, valid link and source inspection do **not** replace live Unreal Editor tests. The verification record should preserve failures, exact versions and screenshots.

[Roadmap](ROADMAP.md) · [Media Inventory](MEDIA_INVENTORY.md) · [Verification Matrix](VERIFICATION_MATRIX.md)
