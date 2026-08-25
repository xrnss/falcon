package club.rnss.launcher.injection.network.utils

import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.charset.StandardCharsets


class Reader {
    private var le = false
    var view: ByteBuffer = ByteBuffer.allocate(1000)
    private var offset = 0

    constructor()
    constructor(view: ByteBuffer, offset: Int, littleEndian: Boolean) {
        le = littleEndian
        this.view = view
        this.offset = offset
        if (littleEndian) this.view.order(ByteOrder.LITTLE_ENDIAN)
    }

    val uint8: Int
        get() = view.get(offset++).toInt()
    val uint16: Short
        get() = view.getShort(2.let { offset += it; offset } - 2)
    val int32: Int
        get() = view.getInt(4.let { offset += it; offset } - 4)
    val uint32: Int
        get() =//return this.view.getLong((this.offset += 4) - 4);
            view.getInt(4.let { offset += it; offset } - 4)
    val float64: Double
        get() = view.getDouble(8.let { offset += it; offset } - 8)
    val stringUTF8: String
        get() {
            var s = ""
            val b = ByteArray(1)
            while (view.get(offset++).also { b[0] = it }.toInt() != 0) {
                s += String(b, StandardCharsets.UTF_8)
            }
            return s
        }
}