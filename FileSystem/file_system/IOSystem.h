//
// Created by Ярослава Левчук on 03.04.2021.
//
#pragma once

class IOSystem {
    private:
        const int L=64;
        const int B=64;
        char **ldisk;

    public:

        IOSystem();
        ~IOSystem();
        void read_block (int i, char *p) const;
        void write_block(int i, char *p);

};


