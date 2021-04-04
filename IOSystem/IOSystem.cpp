//
// Created by Ярослава Левчук on 03.04.2021.
//

#include <iostream>
#include "IOSystem.h"

IOSystem::IOSystem(){
        ldisk = new char*[L];
        for (int j = 0; j < L; j++){
            ldisk[j]=new char[B];
            for(int i=0;i<B;i++)
            ldisk[j][i]={'0'};

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