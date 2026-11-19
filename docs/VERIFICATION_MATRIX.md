# Hue v1.0.0 Verification Matrix

The following scenarios are **not executed in this documentation pass**. A source review is not a passing Unreal Editor test.

| ID | Scenario | Expected behavior | Status |
| --- | --- | --- | --- |
| HUE-01 | Open a Blueprint editor after enabling Hue | Hue tab is available and can be docked | Not run |
| HUE-02 | Set each of four channels on a normal supported node | Independent header, header text, body and body text colors | Not run |
| HUE-03 | Apply Instance, Category and Global overrides to one node | Per-channel precedence resolves independently | Not run |
| HUE-04 | Clear one Instance channel | Lower-priority Category/Global/default source returns | Not run |
| HUE-05 | Clear All Overrides in one scope | All four explicit values in that scope removed | Not run |
| HUE-06 | Change parent and child nested Categories | Closest ancestor per-channel inheritance works | Not run |
| HUE-07 | Rename a styled Category and nested children | Applicable styles migrate rather than being dropped | Not run |
| HUE-08 | Move a single member to a different Category | Old Category style does not migrate as if renamed | Not run |
| HUE-09 | Style Global Variable on Get and Set nodes | Shared logical member identity resolves consistently | Not run |
| HUE-10 | Batch select compatible/unsupported nodes | Unsupported nodes skipped, target counts and Multiple Values correct | Not run |
| HUE-11 | Undo one multi-selection color edit | All affected targets revert in one transaction | Not run |
| HUE-12 | Save/reopen Blueprint with Instance and Category changes | Blueprint-owned styles restored | Not run |
| HUE-13 | Save/reopen project with Global styles | Project editor configuration restores global rules | Not run |
| HUE-14 | Style specialized supported K2 nodes | Native controls remain usable; visuals update safely | Not run |
| HUE-15 | Select reroute/Documentation nodes | No inappropriate styling options or native breakage | Not run |
| HUE-16 | Right-click compatible node in multi-selection | Hue menu respects supported selected scope | Not run |
| HUE-17 | Site navigation on desktop/mobile | Five sections, Design/Technical, search and media fit viewport | Not run |
| HUE-18 | Search terms found inside Markdown body | Search returns relevant content excerpt | Not run |
| HUE-19 | Check images/GIF placeholders and caption lines | All intended paths and types render correctly | Not run |

Record exact UE version, Hue build, Blueprint node class, steps, actual results, Output Log and screenshot/GIF evidence before marking a test Passed.
