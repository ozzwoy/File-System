#pragma once

namespace Utils {

    unsigned bytesToUint(const char* bytes, int size) {
        int number = 0;

        for(int i = 0; i < size; i++ )
            ((unsigned char*)(&number))[i] = ((unsigned char*)bytes)[i];

        return number;
    }

    char* uintToBytes(unsigned number) {
        char* bytes = new char[sizeof(unsigned)];

        for(int i = 0; i < sizeof(unsigned); i++ )
            ((unsigned char*)bytes)[i] = ((unsigned char*)(&number))[i];

        return bytes;
    }

}
