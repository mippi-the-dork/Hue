# Troubleshooting Hue — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Hue tab missing

Verify Hue is enabled in Edit → Plugins and restart the Editor. Open a **Blueprint Editor**, not the World Outliner, and restore the Hue tab if it was closed.

## Selected node unsupported

Try a normal Call Function node. Hue intentionally skips reroute nodes, Documentation nodes and other native/custom presentations without a confirmed visual path.

## Category unavailable

Check that the underlying function, variable, macro or event belongs to a user-defined **My Blueprint** Category in the defining Blueprint. Action-menu groups alone don't qualify.

## Clear didn't restore Unreal default

Check the effective source label. If a parent Category or Global rule still owns the channel, removing the current override reveals that inherited value.

## Colors differ after renaming

Confirm whether you renamed a Category or moved an individual member. Category rename migration and member reorganization are different; inspect the affected Blueprint metadata and save state.

## Global style isn't shared by another node

Two visually identical titles may refer to different logical node identities. Check the exact function, variable or node family rather than relying on the displayed label.

## Styles don't survive reload

Save the owning/defining Blueprint for Instance/Category changes. For Global styles, verify project Hue config is included in your expected sharing workflow.

## Need to report a bug?

Include Hue/UE versions, exact node class, selected scope/channel, whether the node uses a specialized widget, steps, screenshots and Output Log details. Use [Hue Issues](https://github.com/mippi-the-dork/Hue/issues).

---

For native implementation details, [read Technical](TECHNICAL.md).
