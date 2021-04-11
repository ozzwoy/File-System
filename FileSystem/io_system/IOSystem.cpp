//
// Created by Ярослава Левчук on 03.04.2021.
//

#include "IOSystem.h"

IOSystem::IOSystem() {
    ldisk = new char*[NUM_OF_BLOCKS];
    for (int i = 0; i < NUM_OF_BLOCKS; i++) {
        ldisk[i] = new char[BLOCK_SIZE];
    }
}

IOSystem::~IOSystem() {
    for (int i = 0; i < NUM_OF_BLOCKS; i++) {
        delete[] ldisk[i];
    }
    delete[] ldisk;
}

void IOSystem::readBlock (int i, char *p) const {
    for(int j = 0; j < BLOCK_SIZE; j++) {
        p[j] = ldisk[i][j];
    }
};

void IOSystem::writeBlock(int i, const char *p) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
        ldisk[i][j] = p[j];
    }
}