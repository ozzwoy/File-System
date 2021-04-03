//
// Created by Ярослава Левчук on 03.04.2021.
//

#include <iostream>
#include "IOSystem.h"

IOSystem::IOSystem(){
    for (int i = 0; i < L; i++){
        ldisk = new char*[i];
        for (int j = 0; j < B; j++){
            ldisk[i]=new char[j];
            ldisk[i][j]='0';
        }
    }

}

IOSystem::~IOSystem(){
    for (int i = 0; i < L; i++)
            delete[]ldisk[i];
    delete[] ldisk;
}

void IOSystem::read_block (int i, char *p) const {
        for(int j=0;j<64;j++)
            p[j]= ldisk[i][j];

};

void IOSystem::write_block(int i, char *p) {

    for (int j = 0; j < 64; j++)
            ldisk[i][j] = p[j];

};