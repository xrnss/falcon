package club.rnss.launcher.network

import club.rnss.launcher.injection.network.utils.Reader
import org.java_websocket.WebSocket
import org.java_websocket.handshake.ClientHandshake
import org.java_websocket.server.WebSocketServer
import java.lang.Exception
import java.net.InetSocketAddress
import java.nio.ByteBuffer

class Server(port: Int) : WebSocketServer(InetSocketAddress(port)) {
    override fun onOpen(conn: WebSocket?, handshake: ClientHandshake?) {
        println("[SERVER] Connection opened")
    }

    override fun onClose(
        conn: WebSocket?,
        code: Int,
        reason: String?,
        remote: Boolean
    ) {
        println("[SERVER] Connection closed")
    }

    override fun onMessage(conn: WebSocket?, message: String?) {
        println("[SERVER] Message: $message")
    }

    override fun onMessage(conn: WebSocket?, message: ByteBuffer?) {
        message?.let { Protocol.handlePacket(Reader(it, 0, false), conn) }
    }

    override fun onError(conn: WebSocket?, ex: Exception?) {
        println("[SERVER] Connection error")
        ex?.printStackTrace()
    }

    override fun onStart() {
        println("[SERVER] Server started")
    }
}