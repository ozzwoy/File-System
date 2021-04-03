#pragma once

namespace Utils {

    char* intToBytes(int number) {
        char* bytes = new char[sizeof(int)];

        for (int i = 0; i < sizeof(int); i++) {
            ((unsigned char*)bytes)[i] = ((unsigned char*)(&number))[i];
        }

        return bytes;
    }

    int bytesToInt32(const char* bytes) {
        int number = 0;

        for (int i = 0; i < 4; i++) {
            ((unsigned char*)(&number))[i] = ((unsigned char*)bytes)[i];
        }

        return number;
    }

}
