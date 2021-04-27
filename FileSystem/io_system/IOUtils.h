//
// Created by Ярослава Левчук on 04.04.2021.
//

#ifndef FILE_SYSTEM_IOUTILS_H
#define FILE_SYSTEM_IOUTILS_H

#include <fstream>
#include "IOSystem.h"


namespace IOUtils {

    void save(IOSystem const &ldisk, const char *path) {
        std::ofstream out(path);
        char *block = new char[IOSystem::BLOCK_SIZE];

        if (out.is_open()) {
            for (int i = 0; i < IOSystem::NUM_OF_BLOCKS; i++) {
                ldisk.readBlock(i, block);
                out << block << std::endl;
            }
        }

        delete[] block;
        out.close();
    }

    void restore(IOSystem &ldisk, const char *path){
        std::ifstream in(path);
        char *block = new char[IOSystem::BLOCK_SIZE];

        if (in.is_open()) {
            for (int i = 0; i < IOSystem::NUM_OF_BLOCKS; i++) {
                in.get(block, IOSystem::BLOCK_SIZE);
                ldisk.writeBlock(i, block);
            }
        }

        delete[] block;
        in.close();
    }

}


#endif //FILE_SYSTEM_IOUTILS_H