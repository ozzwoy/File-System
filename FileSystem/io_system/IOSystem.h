//
// Created by Ярослава Левчук on 03.04.2021.
//

#ifndef IO_SYSTEM_IOSYSTEM_H
#define IO_SYSTEM_IOSYSTEM_H


class IOSystem {
public:
    static const int NUM_OF_BLOCKS = 64;
    static const int BLOCK_SIZE = 64;

private:
    char **ldisk;

public:
    IOSystem();

    ~IOSystem();

    void read_block(int i, char *p) const;

    void write_block(int i, const char *p);
};


#endif //IO_SYSTEM_IOSYSTEM_H