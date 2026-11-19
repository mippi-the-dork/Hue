# Nested Category Inheritance — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Inheritance follows the hierarchy

Unreal uses the vertical bar in My Blueprint category paths: Combat, Combat|Weapons, Combat|Weapons|Melee. A style applied to Combat can be inherited by either child when a closer override is absent.

## Nearest parent, one channel at a time

If Combat defines a red header and gray body, while Combat|Weapons defines an orange header only, a Melee node can have **orange Header Color from Weapons** and **gray Body Color from Combat**.

## Try it

1. Style a parent category's Header Color.
2. Confirm a child member inherits it.
3. Override the Header Color on the nearer child category.
4. Clear that child override; the original parent's header should immediately return.

## What can surprise you

Editing a parent changes every eligible descendant without a closer override for that particular channel. Preview the current source label and the affected hierarchy before changing a heavily reused parent category.

---

For native implementation detail, [read Technical](TECHNICAL.md).
