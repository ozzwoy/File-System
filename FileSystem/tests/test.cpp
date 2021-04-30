#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <cstring>
#include "../file_system/entities/BitMap.h"
#include "../file_system/FileSystem.h"

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

    SUBCASE("create file"){
        FileSystem fs;
        std::string name;
        name = "tests";
        REQUIRE_THROWS_WITH(fs.createFile(name.c_str()), "invalid operation!");
        name = "";
        REQUIRE_THROWS_WITH(fs.createFile(name.c_str()), "invalid operation!");
        name = "test";
        REQUIRE_NOTHROW(fs.createFile(name.c_str()));

    }

    SUBCASE("destroy file"){
        FileSystem fs;
        std::string name;
        name = "test";
        REQUIRE_THROWS_WITH(fs.destroyFile(name.c_str()), "invalid operation!");
        fs.createFile(name.c_str());
        REQUIRE_NOTHROW(fs.destroyFile(name.c_str()));
    }

    SUBCASE("get directory"){
        FileSystem fs;
        std::vector<std::string> filenames;
        filenames = fs.directory();
        REQUIRE(filenames.empty());

        std::string name;
        name = "test";
        fs.createFile(name.c_str());
        filenames = fs.directory();
        REQUIRE(filenames.size() == 1);
        REQUIRE(filenames[0] == "test 0");
    }

}