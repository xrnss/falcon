package club.rnss.launcher.gui

import java.awt.BorderLayout
import java.awt.Color
import java.awt.Dimension
import java.awt.Graphics
import java.awt.Graphics2D
import java.awt.Image
import java.awt.Point
import java.awt.RenderingHints
import java.awt.event.MouseAdapter
import java.awt.event.MouseEvent
import java.awt.geom.Area
import java.awt.geom.RoundRectangle2D
import java.io.IOException
import javax.imageio.ImageIO
import javax.swing.BorderFactory
import javax.swing.JComponent
import javax.swing.JFrame
import javax.swing.JPanel
import javax.swing.JProgressBar
import javax.swing.SwingUtilities
import javax.swing.plaf.basic.BasicProgressBarUI

class MainGui {
    var storedProgressBar: JProgressBar? = null
    var frameInst: JFrame? = null

    init {
        SwingUtilities.invokeLater {
            initUI();
        }
    }

    fun initUI() {
        // Create frame
        val frame = JFrame("Falcon")
        frame.defaultCloseOperation = JFrame.EXIT_ON_CLOSE

        // Pos + Size
        frame.setSize(800, 450)
        frame.isUndecorated = true
        frame.setLocationRelativeTo(null) // Centered

        // Appearance
        frame.contentPane.background = Color.decode("#151416")
        frame.shape = Area(RoundRectangle2D.Float(0f, 0f, frame.width.toFloat(), frame.height.toFloat(), 20f, 20f))
        frame.add(ImagePanel("/FalconLogo.png", 275, 78).apply { isOpaque = false })

        // Progress Bar
        storedProgressBar = JProgressBar(0, 100).apply {
            isStringPainted = false
            value = 40
            font = Fonts.CUSTOM
            isBorderPainted = false
            foreground = Color.GRAY
            background = Color.decode("#151416")
            ui = RoundedProgressBarUI()
            size = Dimension(405, 18)
            preferredSize = Dimension(405, 18)
            string = " "
        }

        val progressPanel = JPanel().apply {
            size = Dimension(405, 18)
            isOpaque = false
            border = BorderFactory.createEmptyBorder(0, 0, 175, 0)
            add(storedProgressBar)
        }

        frame.add(progressPanel, BorderLayout.SOUTH)

        // Draggable
        makeDraggable(frame)

        frameInst = frame
        frame.isVisible = true
    }

    private fun makeDraggable(frame: JFrame) {
        var mouseDownCompCoords: Point? = null

        frame.addMouseListener(object : MouseAdapter() {
            override fun mousePressed(e: MouseEvent) {
                mouseDownCompCoords = e.point
            }
        })

        frame.addMouseMotionListener(object : MouseAdapter() {
            override fun mouseDragged(e: MouseEvent) {
                mouseDownCompCoords?.let {
                    val frameLocation = frame.location
                    val xDiff = e.point.x - it.x
                    val yDiff = e.point.y - it.y
                    frame.setLocation(frameLocation.x + xDiff, frameLocation.y + yDiff)
                }
            }
        })
    }

    class ImagePanel(private val imagePath: String, private val targetWidth: Int, private val targetHeight: Int) : JPanel() {
        private var image: Image? = null

        init {
            try {
                // Load the image from resources
                val url = javaClass.getResource(imagePath)
                image = ImageIO.read(url)
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }

        override fun getPreferredSize(): Dimension {
            return Dimension(targetWidth, targetHeight)
        }

        override fun paintComponent(g: Graphics) {
            super.paintComponent(g)
            val g2d = g.create() as Graphics2D
            g2d.drawImage(image, (width - targetWidth) / 2, 150, targetWidth, targetHeight, this)
            g2d.dispose()
        }
    }
}

internal class RoundedProgressBarUI : BasicProgressBarUI() {
    override fun paintDeterminate(g: Graphics, c: JComponent?) {
        val g2d = g.create() as Graphics2D
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON)
        val barRectWidth = progressBar.width
        val barRectHeight = progressBar.height

        // Draw the background
        g2d.color = Color.decode("#353536")
        g2d.fillRoundRect(0, 0, barRectWidth, barRectHeight, ARC_SIZE, ARC_SIZE)

        // Draw the filled part
        val amountFull = getAmountFull(null, barRectWidth, barRectHeight)
        g2d.color = progressBar.foreground
        g2d.fillRoundRect(0, 0, amountFull, barRectHeight, ARC_SIZE, ARC_SIZE)

        val progressBar = c as JProgressBar
        val text = progressBar.string // Get the text from the progress bar

        if (text == null || text.isEmpty()) {
            return
        }

        val metrics = progressBar.getFontMetrics(progressBar.font)
        val x = (progressBar.width - metrics.stringWidth(text)) / 2
        val y = (progressBar.height - metrics.height) / 2 + metrics.ascent - 1
        g.color = Color.WHITE
        g.drawString(text, x, y)

        g2d.dispose()
    }

    override fun paintIndeterminate(g: Graphics?, c: JComponent?) {
        // Customize the indeterminate painting if needed
        super.paintIndeterminate(g, c)
    }

    companion object {
        private const val ARC_SIZE = 15 // Adjust this value to control roundness
    }
}