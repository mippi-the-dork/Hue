# Supported and Unsupported Node Presentations — Design

**Hue v1.0.0 · Unreal Engine 5.8.x · Win64 · Editor only**

## Supported families

Hue supports normal compatible K2 nodes and various specialized presentation families, including calls, variables, events, operators, switches, sequences, timelines and composites where a safe coloring path exists.

## Why some nodes are excluded

Hue must preserve the correct native widget. It only offers styling when it has confirmed a compatible presentation. A valid Blueprint node is not automatically a Hue-compatible *visual* node.

## Intentionally unsupported

**Reroute nodes** have no meaningful four-channel header/body surface. **Documentation nodes** are intentionally unsupported in v1.0.0. Custom visual widgets that Hue cannot safely decorate remain native.

## Native controls stay functional

Numeric fields, checkboxes, dropdowns, object/class pickers and other interactive native controls do not necessarily adopt Hue Body Text Color.

## Help diagnose an unsupported node

Include its exact node class, Blueprint graph context, Unreal version and full node screenshot in a bug report. Try an ordinary function call for comparison.

---

For native implementation detail, [read Technical](TECHNICAL.md).
