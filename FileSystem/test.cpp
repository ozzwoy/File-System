#include <iostream>
#include "FileSystem/Utils.h"

int main() {
    char* bytes = Utils::uintToBytes(5);
    std::cout << Utils::bytesToUint(bytes, 1) << std::endl;
    return 0;
}