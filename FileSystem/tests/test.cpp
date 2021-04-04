#include <iostream>
#include "../file_system/utils/Utils.h"
#include "../io_system/IOSystem.h"
#include "../io_system/IOSystemUtils.h"


int main() {
    char* bytes = Utils::intToBytes(-236862);
    std::cout << Utils::bytesToInt32(bytes) << std::endl;

    IOSystem *disk= new IOSystem();
   char temp[10]={'d','v','d','f','b','d','d','b','f'};
    char *temp2=new char[64];
    disk->write_block(0,temp);
    disk->read_block(0,temp2);
    std::cout<<temp2;

    IOSystemUtils::save(*disk,"D:\\3kurs_2\\OPK\\File-System-levchuk_branch\\FileSystem\\tests\\test.txt");
    IOSystemUtils::init(*disk,"D:\\3kurs_2\\OPK\\File-System-levchuk_branch\\FileSystem\\tests\\test.txt");

    return 0;
}