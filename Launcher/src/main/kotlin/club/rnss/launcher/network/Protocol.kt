package club.rnss.launcher.network

import club.rnss.launcher.injection.network.utils.Reader
import club.rnss.launcher.injection.network.utils.Writer
import org.java_websocket.WebSocket
import java.io.File
import java.util.Base64
import java.util.jar.JarFile

object Protocol {
    fun handlePacket(reader: Reader, conn: WebSocket?) {
        when (reader.uint8) {
            1 -> { // Handshake [B]
                // We can now access any data the client wished to communicate during the Handshake
                // e.g. HWIDs, Client-Types, JVM Info

                val clientType = reader.stringUTF8
                // ...

                // Go to directory of this executing JAR and find Bootstrap.jar
                // Note: This should be changed. Maybe you want to get the class names/bytes from elsewhere like a server request.

                val currentJar = File(object {}.javaClass.protectionDomain.codeSource.location.toURI())
                val bootstrapJar = File(currentJar.parentFile, "Bootstrap.jar")

                val classes = mutableListOf<Pair<String, ByteArray>>()
                JarFile(bootstrapJar).use { jar ->
                    jar.entries().asSequence().filter { it.name.endsWith(".class") }.forEach { entry ->
                        val className = entry.name.removeSuffix(".class").replace("/", ".")
                        val bytes = jar.getInputStream(entry).readBytes()

                        classes.add(className to bytes)
                    }
                }

                val writer = Writer()
                writer.setPacketId(2)
                writer.setUint32(classes.size)

                classes.forEach { (className, classBytes) ->
                    val bytes = String(Base64.getEncoder().encode(classBytes))

                    writer.setStringUTF8(className)
                    writer.setStringUTF8(bytes)
                }

                // Note: Change this if you modify club/rnss/bootstrap/Bootstrap or the bootstrap method; you may have to update the signature.
                writer.setStringUTF8("club/rnss/bootstrap/Bootstrap") // Class Name
                writer.setStringUTF8("initiate") // Method Name
                writer.setStringUTF8("()V") // Method Signature

                conn?.send(writer.build())
            }
            2 -> { // Class [B]
                // Classes successfully loaded
                // Disconnect if desired?
            }
        }
    }
}