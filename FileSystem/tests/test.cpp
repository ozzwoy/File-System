#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <cstring>
#include <stdexcept>
#include <string>
#include "../file_system/entities/BitMap.h"
#include "../file_system/FileSystem.h"

TEST_CASE("Entities") {

    SUBCASE("BitMap"){
        char buffer[BitMap::SIZE + 1];
        buffer[BitMap::SIZE] = '\0';
        BitMap bitmap;
        bitmap.parse("abcdefgh");

        REQUIRE( bitmap.isBitSet(1) );

        bitmap.setBitValue(1, false);
        bitmap.copyBytes(buffer);
        REQUIRE( strcmp(buffer,"!bcdefgh") == 0 );

        bitmap.setBitValue(1, true);
        bitmap.copyBytes(buffer);
        REQUIRE( strcmp(buffer,"abcdefgh") == 0 );
    }

    SUBCASE("DirectoryEntry") {
        DirectoryEntry directoryEntry;
        char name[DirectoryEntry::MAX_FILE_NAME_SIZE + 1];
        char buffer[DirectoryEntry::SIZE + 1];
        buffer[DirectoryEntry::SIZE] = '\0';

        directoryEntry.parse("file\0\0\0\0");
        directoryEntry.copyFileName(name);
        REQUIRE(std::strcmp(name, "file") == 0);
        REQUIRE(directoryEntry.getDescriptorIndex() == 0);

        directoryEntry.setFileName("new");
        directoryEntry.setDescriptorIndex(7);
        directoryEntry.copyBytes(buffer);
        REQUIRE(std::strcmp(buffer, "new\0\0\0\0\a") == 0);
    }

    SUBCASE("Descriptor") {
        Descriptor descriptor;
        char buffer[Descriptor::SIZE + 1];
        buffer[Descriptor::SIZE] = '\0';

        descriptor.parse("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
        REQUIRE(descriptor.getFileSize() == 0);
        for (int i = 0; i < Descriptor::NUM_OF_BLOCKS; i++) {
            REQUIRE(descriptor.getBlockIndex(i) == 0);
        }

        descriptor.setFileSize(7);
        descriptor.setBlockIndex(1, 7);
        descriptor.copyBytes(buffer);
        REQUIRE(std::strcmp(buffer, "\0\0\0\a\0\0\0\0\0\0\0\a\0\0\0\0\0"));
    }

}

TEST_CASE("File System") {
    FileSystem fs;

    SUBCASE("File name") {
        std::string name;
        for (int i = 0; i <= FileSystem::MAX_FILE_NAME_SIZE; i++) {
            name.push_back('a');
        }
        CHECK_THROWS_AS(fs.createFile(name.c_str()), std::invalid_argument);
        name = "";
        CHECK_THROWS_AS(fs.createFile(name.c_str()), std::invalid_argument);
        name = "okay";
        CHECK_NOTHROW(fs.createFile(name.c_str()));
    }

    SUBCASE("Create") {
        for (int i = 0; i < FileSystem::MAX_NUM_OF_FILES - 1; i++) {
            fs.createFile(std::to_string(i).c_str());
        }
        REQUIRE_THROWS_WITH(fs.createFile("0"), "File with name \"0\" already exists.");

        fs.createFile(std::to_string(FileSystem::MAX_NUM_OF_FILES - 1).c_str());
        REQUIRE_THROWS_AS(fs.createFile(std::to_string(FileSystem::MAX_NUM_OF_FILES).c_str()), std::length_error);
    }

    SUBCASE("Destroy"){
        std::string name("nope");
        REQUIRE_THROWS_WITH(fs.destroyFile(name.c_str()), "File with name \"nope\" does not exist.");
        fs.createFile(name.c_str());
        REQUIRE_NOTHROW(fs.destroyFile(name.c_str()));
    }

    SUBCASE("Open") {
        std::string name("nope");
        REQUIRE_THROWS_WITH(fs.open(name.c_str()), "File with name \"nope\" does not exist.");

        // open same file twice
        fs.createFile(name.c_str());
        int index1;
        REQUIRE_NOTHROW(index1 = fs.open(name.c_str()));
        int index2 = fs.open(name.c_str());
        REQUIRE_EQ(index1, index2);

        // open files over limit
        for (int i = 2; i < FileSystem::MAX_FILES_OPENED; i++) {
            name = std::to_string(i);
            fs.createFile(name.c_str());
            fs.open(name.c_str());
        }
        name = std::to_string(FileSystem::MAX_FILES_OPENED);
        fs.createFile(name.c_str());
        REQUIRE_THROWS_AS(fs.open(name.c_str()), std::length_error);
    }

    SUBCASE("Destroy opened file") {
        std::string name("file");
        fs.createFile(name.c_str());
        fs.open(name.c_str());
        fs.destroyFile(name.c_str());
        REQUIRE_THROWS_AS(fs.open(name.c_str()), std::invalid_argument);
    }

    SUBCASE("Close") {
        // cannot close directory file
        REQUIRE_THROWS(fs.close(0));

        std::string name("file");
        fs.createFile(name.c_str());
        int index = fs.open(name.c_str());
        REQUIRE_NOTHROW(fs.close(index));
        REQUIRE_THROWS(fs.close(index));
    }

    SUBCASE("Write & read & lseek") {
        std::string name("file");
        fs.createFile(name.c_str());
        int index = fs.open(name.c_str());

        const int num = 100;
        char buffer1[num];
        char buffer2[num];
        for (int i = 0; i < num - 1; i++) {
            buffer1[i] = 'A';
        }
        buffer1[num - 1] = '\0';

        REQUIRE(fs.write(index, buffer1, num) == num);
        REQUIRE(fs.read(index, buffer2, num) == 0);
        fs.lseek(index, 0);
        REQUIRE(fs.read(index, buffer2, num) == num);
        REQUIRE(std::strcmp(buffer1, buffer2) == 0);

        REQUIRE_THROWS(fs.lseek(index, num + 1));

        // rewrite
        for (int i = 0; i < num - 1; i++) {
            buffer1[i] = 'B';
        }
        int pos = 30;
        fs.lseek(index, pos);
        REQUIRE(fs.write(index, buffer1, num) == num);
        fs.lseek(index, pos);
        REQUIRE(fs.read(index, buffer2, num) == num);
        REQUIRE(std::strcmp(buffer1, buffer2) == 0);

        // write over file size limit
        char buffer3[FileSystem::MAX_FILE_SIZE + 10];
        fs.lseek(index, 0);
        REQUIRE(fs.write(index, buffer3, FileSystem::MAX_FILE_SIZE + 10) == FileSystem::MAX_FILE_SIZE);
        fs.lseek(index, 0);
        REQUIRE(fs.read(index, buffer3, FileSystem::MAX_FILE_SIZE + 10) == FileSystem::MAX_FILE_SIZE);
    }

    SUBCASE("Directory") {
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