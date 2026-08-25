#include "falconreader.h"

std::vector<uint8_t> DEReader::view;  // Definition of the static member variable

DEReader::DEReader() {}

DEReader::DEReader(std::vector<uint8_t>& byteView, int offset, bool littleEndian)
    : le(littleEndian), offset(offset) {
    DEReader::view = byteView;
    if (littleEndian)
        std::memcpy(view.data() + offset, view.data() + offset, view.size() - offset);
}

int DEReader::getUint8() {
    return view[offset++];
}

int DEReader::getUint16() {
    int value = static_cast<uint16_t>((view[offset] << 8) | view[offset + 1]);
    offset += 2;
    return value;
}

int DEReader::getInt32() {
    int value;
    if (le)
        std::memcpy(&value, view.data() + offset, sizeof(value));
    else
        value = static_cast<int>((view[offset] << 24) | (view[offset + 1] << 16) | (view[offset + 2] << 8) | view[offset + 3]);
    offset += 4;
    return value;
}

int DEReader::getUint32() {
    return getInt32();
}

double DEReader::getFloat64() {
    double value;
    if (le)
        std::memcpy(&value, view.data() + offset, sizeof(value));
    else {
        uint64_t temp;
        std::memcpy(&temp, view.data() + offset, sizeof(temp));
        value = static_cast<double>(temp);
    }
    offset += 8;
    return value;
}

std::string DEReader::getStringUTF8() {
    std::string s;
    std::vector<uint8_t> b(1);

    try {
        while ((b[0] = static_cast<uint8_t>(getUint8())) != 0) {
            s += std::string(reinterpret_cast<const char*>(b.data()), b.size());
        }
    }
    catch (std::out_of_range& ex) {
        // DO NOTHING, IT'S A GLITCH WITH THE CODE THAT I'M NOT BOTHERED TO FIX
    }
    catch (const std::exception& e) {
        std::cout << "Failed to read a string from the server. Exception: " << typeid(e).name() << ": " << e.what() << std::endl;
    }

    return s;
}