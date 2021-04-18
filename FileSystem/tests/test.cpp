#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <cstring>
#include "../file_system/entities/BitMap.h"
#include "../file_system/FileSystem.h"
#include "../io_system/IOSystem.h"

TEST_CASE("File System"){

    SUBCASE("BitMap"){
        char buffer[9];
        buffer[8] = '\0';
        BitMap bitmap;
        bitmap.parse("abcdefgh");

        REQUIRE( bitmap.isBitSet(1) );

        bitmap.setBitValue(1, 0);
        bitmap.copyBytes(buffer);
        REQUIRE( strcmp(buffer,"!bcdefgh") == 0 );

        bitmap.setBitValue(1, 1);
        bitmap.copyBytes(buffer);
        REQUIRE( strcmp(buffer,"abcdefgh") == 0 );
    }

    SUBCASE("create File System"){
        auto *io = new IOSystem;
        FileSystem fs(*io);
        REQUIRE (fs.createFile("test") == 0);
        REQUIRE (fs.destroyFile("test") == 0);
    }
}