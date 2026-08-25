package club.rnss.launcher.injection.network.utils

import java.nio.ByteOrder
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

class Writer {
    var offset = 0
    private lateinit var ex: IntArray
    private var le = false
    private var b = IntArray(1)

    constructor()
    constructor(littleEndian: Boolean) {
        le = littleEndian
    }

    fun setPacketId(pId: Int) {
        b[0] = pId
    }

    fun setUint8(a: Int) {
        if (a >= 0 && a < 256) {
            extend(1)
            ex[offset] = a
            b = ex
        }
    }

    fun setInt32(a: Int) {
        val bytes = ByteArray(4)
        if (le) ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN).putInt(a) else ByteBuffer.wrap(bytes).order(
            ByteOrder.BIG_ENDIAN
        ).putInt(
            a
        )
        extend(4)
        for (p in bytes) ex[offset++] = p.toInt()
        b = ex
    }

    fun setUint32(a: Int) {
        val bytes = ByteArray(4)
        if (le) ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN).putInt(a) else ByteBuffer.wrap(bytes).order(
            ByteOrder.BIG_ENDIAN
        ).putInt(
            a
        )
        extend(4)
        for (p in bytes) ex[offset++] = p.toInt()
        b = ex
    }

    fun setStringUTF8(s: String) {
        val bytes: ByteArray = s.toByteArray(StandardCharsets.UTF_8)
        extend(1 + bytes.size)
        for (p in bytes) ex[offset++] = p.toInt()
        ex[offset] = 0
        b = ex
        /* byte[] bytes = new byte[s.toCharArray().length];
        ByteBuffer buffer = ByteBuffer.wrap(bytes).order(ByteOrder.LITTLE_ENDIAN);
        int o = 0;
        for (char c : s.toCharArray()) buffer.putChar(o, c); */
    }

    fun push(vararg args: Int) {
        for (p in args) setUint8(p)
    }

    fun extend(exSize: Int) {
        ex = IntArray(b.size + exSize)
        offset = 0
        for (p in b) ex[offset++] = p
    }

    fun build(): ByteBuffer {
        val buffer: ByteBuffer = ByteBuffer.allocate(b.size)
        var o = 0
        for (p in b) buffer.put(o++, p.toByte())
        return buffer
    }
}