#ifndef FILE_SYSTEM_NUMERICUTILS_H
#define FILE_SYSTEM_NUMERICUTILS_H


namespace Utils {

    inline void intToBytes(int number, char *buffer) {
        for (size_t i = 0; i < sizeof(int); i++) {
            ((unsigned char*)buffer)[i] = ((unsigned char*)(&number))[i];
        }
    }

    inline int bytesToInt(const char* bytes) {
        int number = 0;

        for (size_t i = 0; i < sizeof(int); i++) {
            ((unsigned char*)(&number))[i] = ((unsigned char*)bytes)[i];
        }

        return number;
    }

}


#endif //FILE_SYSTEM_NUMERICUTILS_H
