package club.rnss.launcher

import club.rnss.launcher.gui.MainGui
import club.rnss.launcher.injection.Injector
import club.rnss.launcher.network.Server
import java.net.ServerSocket

var mainGui: MainGui? = null
var server: Server? = null

// Find free port for communication with DLL once inside JVM
val socketPort by lazy {
    for (port in 1000..Short.MAX_VALUE) {
        if (runCatching { ServerSocket(port).close() }.isSuccess)
            return@lazy port
    }
    error("could not find free port.")
}

fun main(args: Array<String>) {
    // Init GUI
    mainGui = MainGui()
    server = Server(socketPort)
    server?.start()

    // Fetch FalconLoader.dll
    val dllBuffer: ByteArray =
        object {}.javaClass.getResourceAsStream("/club/rnss/launcher/FalconLoader.dll")
            ?.use { it.readBytes() }
            ?: error("DLL resource not found")

    Injector().waitForProcessAndInject(dllBuffer, socketPort);
}
