package club.rnss.launcher.injection

import com.sun.jna.Pointer
import com.sun.jna.ptr.IntByReference
import com.sun.jna.platform.win32.User32

class Injector {
    private val user32 = User32.INSTANCE
    private val windowTargets = arrayOf("LWJGL", "GLFW30") // The target processes (default: Minecraft)

    fun getTargetPid(): Int {
        var pid = IntByReference(-1)

        do {
            for (window in windowTargets) {
                val hWnd = user32.FindWindow(window, null)
                if (hWnd?.pointer != Pointer.NULL) {
                    val windowText = with(CharArray(1024)) {
                        user32.GetWindowText(hWnd, this, size)
                        String(this)
                    }

                    if (windowText.contains("Minecraft")) { // Adjust this if necessary
                        user32.GetWindowThreadProcessId(hWnd, pid)
                        break
                    }
                }
            }

            Thread.sleep(1000)
        } while (pid.value < 0)

        return pid.value
    }

    fun waitForProcessAndInject(buffer: ByteArray? = null, port: Int) {
        val pid = getTargetPid()

        val success = Natives().inject(pid, buffer, port)

        if (success) {
            println("Successfully injected!")
        } else {
            error("Failed to inject!")
        }
    }
}