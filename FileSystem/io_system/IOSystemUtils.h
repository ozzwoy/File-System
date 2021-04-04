//
// Created by Ярослава Левчук on 04.04.2021.
//

#ifndef FILE_SYSTEM_IOSYSTEMUTILS_H
#define FILE_SYSTEM_IOSYSTEMUTILS_H

#include <fstream>
#include "../../IOSystem/IOSystem.h"


namespace IOSystemUtils{
    void save(IOSystem const &ldisk, char *path) {
        std::ofstream out;
        out.open(path);

        if (out.is_open()) {
            char *block = new char[64];
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
            char *block = new char[64];
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
#endif //FILE_SYSTEM_IOSYSTEMUTILS_H
