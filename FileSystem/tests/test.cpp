#include <iostream>
#include "../file_system/utils/Utils.h"

int main() {
    char* bytes = Utils::intToBytes(-236862);
    std::cout << Utils::bytesToInt32(bytes) << std::endl;
    return 0;
}