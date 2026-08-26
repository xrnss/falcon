#ifndef DEREADER_H
#define DEREADER_H

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

class FalconReader {
private:
    bool le;
    static std::vector<uint8_t> view;  // Declaration of the static member variable
    int offset;

public:
    FalconReader();

    FalconReader(std::vector<uint8_t>& byteView, int offset, bool littleEndian);

    int getUint8();

    int getUint16();

    int getInt32();

    int getUint32();

    double getFloat64();

    std::string getStringUTF8();
};

#endif // DEREADER_H