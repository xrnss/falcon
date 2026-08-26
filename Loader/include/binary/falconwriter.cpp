#include <vector>
#include <cstdint>
#include <cstring>
#include <string>

class FalconWriter {
private:
    int offset = 0;
    std::vector<uint8_t> ex;
    bool le;
    std::vector<uint8_t> b;

public:
    FalconWriter() {}

    FalconWriter(bool littleEndian) : le(littleEndian) {}

    void setPacketId(int pId) {
        b = { static_cast<uint8_t>(pId) };
    }

    void setUint8(int a) {
        if (a >= 0 && a < 256) {
            extend(1);
            ex[offset] = static_cast<uint8_t>(a);
            b = ex;
        }
    }

    void setInt32(int a) {
        uint8_t bytes[4];
        if (le)
            memcpy(bytes, &a, sizeof(a));
        else {
            bytes[0] = static_cast<uint8_t>((a >> 24) & 0xFF);
            bytes[1] = static_cast<uint8_t>((a >> 16) & 0xFF);
            bytes[2] = static_cast<uint8_t>((a >> 8) & 0xFF);
            bytes[3] = static_cast<uint8_t>(a & 0xFF);
        }

        extend(4);

        for (uint8_t p : bytes)
            ex[offset++] = p;
        b = ex;
    }

    void setUint32(int a) {
        uint8_t bytes[4];
        if (le)
            memcpy(bytes, &a, sizeof(a));
        else {
            bytes[0] = static_cast<uint8_t>((a >> 24) & 0xFF);
            bytes[1] = static_cast<uint8_t>((a >> 16) & 0xFF);
            bytes[2] = static_cast<uint8_t>((a >> 8) & 0xFF);
            bytes[3] = static_cast<uint8_t>(a & 0xFF);
        }

        extend(4);

        for (uint8_t p : bytes)
            ex[offset++] = p;
        b = ex;
    }

    void setStringUTF8(const std::string& s) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(s.c_str());
        int length = s.length();

        extend(1 + length);

        for (int i = 0; i < length; i++)
            ex[offset++] = bytes[i];
        ex[offset] = 0;
        b = ex;
    }

    void push(const std::vector<int>& args) {
        for (int p : args)
            setUint8(p);
    }

    void extend(int exSize) {
        ex.resize(b.size() + exSize);
        offset = 0;
        for (uint8_t p : b)
            ex[offset++] = p;
    }

    std::vector<uint8_t> build() {
        return b;
    }
};