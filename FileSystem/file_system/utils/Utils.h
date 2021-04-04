#ifndef FILE_SYSTEM_UTILS_H
#define FILE_SYSTEM_UTILS_H


namespace Utils {

    inline char* intToBytes(int number) {
        char* bytes = new char[sizeof(int)];

        for (size_t i = 0; i < sizeof(int); i++) {
            ((unsigned char*)bytes)[i] = ((unsigned char*)(&number))[i];
        }

        return bytes;
    }

    inline int bytesToInt32(const char* bytes) {
        int number = 0;

        for (size_t i = 0; i < 4; i++) {
            ((unsigned char*)(&number))[i] = ((unsigned char*)bytes)[i];
        }

        return number;
    }

}


#endif //FILE_SYSTEM_UTILS_H