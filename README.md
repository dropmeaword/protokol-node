## Protokol Nodes

Code for generic mesh networked RF nano-based nodes that can interface with physical objects.

### Addressing the nodes in the network

When a node is first started it generates a random 16-bit address, very much like a two number MAC address (e.g. A0:03 ). Each node has a push button on pin 2, pressing the button with set the node address manually. Pressing once makes the node's address 00:01, pressing twice 00:02 and so on. Pressing the button and holding it will generate a new random address.
