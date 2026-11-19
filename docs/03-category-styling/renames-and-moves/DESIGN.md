# Renaming Categories and Moving Members — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Rename a styled category

When a user renames a My Blueprint category, Hue tracks the Blueprint structural change and migrates applicable saved category styling. Nested styled children can move with the renamed parent.

## Moving a member is different

Moving one function from Combat to Utility does **not** rename Combat. The original Combat rule remains; that member can instead pick up Utility's style.

## Confirm a rename

In a test Blueprint, style a parent category and a child, rename the parent through My Blueprint, and inspect their effective colors and source labels. Save and reopen to check persistence.

## A caveat for large reorganizations

Rename detection is event-driven and must distinguish genuine renames from unrelated membership changes. For complex restructures, verify the outcome and use Unreal Undo if needed. Don't assume a whole collection was rekeyed simply because one member moved.

---

For native implementation detail, [read Technical](TECHNICAL.md).
