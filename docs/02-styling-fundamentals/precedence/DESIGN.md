# Style Precedence and Effective Sources — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## How one color is chosen

For each of the four channels, Hue looks for the first applicable rule in this order:

1. **Instance**
2. **Exact Category**
3. **Nearest styled parent Category**, then more distant parents
4. **Global**
5. **Unreal Default**

The order repeats independently for Header Color, Header Text Color, Body Color, and Body Text Color.

## Example: three scopes on one node

Suppose Global supplies a blue Header and dark gray Body. Combat Category supplies a red Header. A specific node's Instance rule supplies yellow Body Text. The result is red Header, inherited native Header Text, dark gray Body, and yellow Body Text.

## Read the current source

The Hue panel can report **Override**, **Category: Combat|Weapons**, **Parent: Combat**, **Global Function/Variable/Event/etc.**, or **Unreal Default**. Those labels tell you *who owns the effective value*, not just what color is visible.

## Clearing reveals the next source

Clearing the Instance Body Text override doesn't force a default if Category or Global still owns that channel. It reveals the next applicable rule. Clearing the Category Header then reveals Global's blue Header.

## Why two same-looking nodes can differ

One may explicitly override a color while the other inherits that same color. In batch editing, Hue may correctly show **Multiple Values** because the underlying sources differ.

---

For the underlying C++ implementation, see [Technical](TECHNICAL.md).
