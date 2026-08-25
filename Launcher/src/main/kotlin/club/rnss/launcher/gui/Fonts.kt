package club.rnss.launcher.gui

import java.awt.Font

object Fonts {
    var CUSTOM: Font? = null

    fun load() {
        try {
            val stream = Fonts::class.java
                .getResourceAsStream("/BenzinRegular.ttf")

            CUSTOM = Font.createFont(
                Font.TRUETYPE_FONT,
                stream
            ).deriveFont(8f)
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
}