#pragma once

#include <fstream>
#include "../IOSystem.h"

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
namespace IOSystemUtils{
    void save(IOSystem const &ldisk, char *path) {
        std::ofstream out;
        out.open(path);


        if (out.is_open()) {
            char *block[64];
            for (int i = 0; i < 64; i++) {
                for (int j = 0; j < 64; j++) {
                    ldisk.read_block(i, *block);
                    out << block;
                }
            }
            out.close();
        }
    }

    void init(IOSystem &ldisk,char *path){
        std::ifstream in(path);

        if (in.is_open())
        {
            char block[64];
            int i=0;
            while (!in.eof())
            {
                in.get(block,64);
                ldisk.write_block(i,block);
                i++;
            }
            in.close();
        }

    }

}
