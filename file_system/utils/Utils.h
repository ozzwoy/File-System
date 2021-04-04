#pragma once

#include <fstream>
#include "../../IOSystem/IOSystem.h"

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
            char *block=new char[64];
            for (int i = 0; i < 64; i++) {
                    ldisk.read_block(i, block);
                    out << block;
                    out<<std::endl;
            }
        }
        out.close();
    }

    void init(IOSystem &ldisk,char *path){
        std::ifstream in(path);

        if (in.is_open())
        {
            char *block=new char[64];
            int i=0;
            while (i<64)
            {
                in.get(block,64);
                ldisk.write_block(i,block);
                i++;
            }

        }
        in.close();
    }

}
