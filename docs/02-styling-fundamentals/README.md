# 02. Styling Fundamentals

**Hue v1.0.0 · Four channels, scope precedence, source reporting and clearing**

The core idea is simple: **each node can inherit color from several levels**, and each of its four visual channels resolves independently.

## The four channels

Header Color sets the primary title-bar tint. Header Text Color colors compatible title text. Body Color affects the node body and certain compact fills. Body Text Color affects eligible labels and execution-pin visuals while leaving native editable controls alone.

## Instance versus Category versus Global

Use Instance for a particular occurrence, Category for supported members grouped under **My Blueprint**, and Global for a supported logical identity throughout the project. A red Category Header can coexist with a blue Global Body and a yellow Instance Body Text.

## Clearing is not resetting everything

Clear removes only the explicit rule at the selected scope and channel. The next applicable inherited color appears. This is why clearing a bright Instance color might reveal another nondefault color rather than an Unreal default.

## Practical advice

Define a restrained shared color language first, then reserve Instance exceptions for important local context. Source labels make it possible to understand where each visible color originates.

## Topics in this section

| Topic | What you will learn | Design | Technical |
| --- | --- | --- | --- |
| **Four Visual Channels** | Identify header, body, title and pin-label styling surfaces | [Design](channels/DESIGN.md) | [Technical](channels/TECHNICAL.md) |
| **Instance, Category and Global** | Choose exactly which nodes should share a style | [Design](scopes/DESIGN.md) | [Technical](scopes/TECHNICAL.md) |
| **Style Precedence and Effective Sources** | Understand inherited colors and the reported current source | [Design](precedence/DESIGN.md) | [Technical](precedence/TECHNICAL.md) |
| **Clearing Overrides and Undo** | Remove overrides without destroying lower-precedence rules | [Design](clearing/DESIGN.md) | [Technical](clearing/TECHNICAL.md) |

Read **Design** for user behavior and **Technical** for the source implementation. The guides complement one another rather than duplicating the same walkthrough.
