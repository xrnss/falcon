package club.rnss.launcher.network

import club.rnss.launcher.injection.network.utils.Reader
import org.java_websocket.WebSocket

object Protocol {
    fun handlePacket(reader: Reader, conn: WebSocket?) {
        when (reader.uint8) {
            1 -> { // Handshake [A]
                // ...
            }
            // ...
        }
    }
}