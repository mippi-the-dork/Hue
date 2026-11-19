# Instance, Category and Global — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Choose the intended reach

**Instance** is for one occurrence in one graph. **Category** targets members associated with a user-defined **My Blueprint** category. **Global** follows a logical node identity throughout the project.

| Scenario | Scope |
| --- | --- |
| This particular damage call is dangerous here | Instance |
| Everything in the Combat member category should be recognizable | Category |
| All calls to this same function should use a consistent color | Global |

## What Category really means

Category is **not** an arbitrary action-menu heading, a C++ class type or the graph node's visible title. It refers to the *My Blueprint* member category of a supported function, event, macro or variable. Some node families have no applicable Category scope.

## What Global really means

Hue can derive global keys for functions, member variables, events, macros, operators and certain node families. A Variable Get and Variable Set for the same member share the **Global Variable** identity. Two nodes with the same screen label do not necessarily share an identity.

## Selecting the scope

Expand the appropriate section in the Hue panel or right-click a compatible node and open its Hue submenu. If a scope is unavailable, choose an applicable node rather than forcing a category that doesn't exist.

## Relationship between scopes

Instance overrides Category, which overrides Global. The choice is evaluated **per channel**, so one node might have an Instance text color, a Category header, and a Global body fill simultaneously.

---

For the underlying C++ implementation, see [Technical](TECHNICAL.md).
