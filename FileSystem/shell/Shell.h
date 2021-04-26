#ifndef FILE_SYSTEM_SHELL_H
#define FILE_SYSTEM_SHELL_H

#include "../file_system/FileSystem.h"

class Shell {
public:
    void start(int argc, char* argv[]);

private:
    FileSystem fs;

    static void incorrectInput();
};


#endif //FILE_SYSTEM_SHELL_H
