#include <iostream>
#include "../file_system/utils/Utils.h"

int main() {
    char* bytes = Utils::intToBytes(-236862);
    std::cout << Utils::bytesToInt32(bytes) << std::endl;

    IOSystem *disk= new IOSystem();
   char temp[10]={'d','v','d','f','b','d','d','b','f'};
    //char *temp2=new char[64];
    disk->write_block(0,temp);
    //disk->read_block(0,temp2);


    return 0;
}