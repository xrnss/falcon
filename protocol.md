# Socket Protocol
| OP Code      | Name | Description |
| ----------- | ----------- | ----------- |
| 1      | Handshake [A/B]       | [A] is sent server->client to request information. [B] is sent client->server to return information + request classes. |
| 2   | Class [A/B]        | [A] is sent server->client as a list of class names (my/java/class) and corresponding bytecode with an inital uint16 of the length of classes in the message. [B] is sent client->server to inform loading is complete. |
| 3   | Resource [A/B] | [A] is sent server->client as a list of resource names and corresponding bytes to be stored in the JVM (e.g. font files, images that may be used by the loaded classes). [B] is sent client_server to inform loading is complete. |
| 8  | Graceful Exit | Sent server->client to request termination of the JVM |
