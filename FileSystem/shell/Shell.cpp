#include "Shell.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

void Shell::start(int argc, char **argv) {
    std::istream *in = &std::cin;
    if (argc == 2) {
        in = new std::ifstream(argv[1]);
    }
    while (true) {
        std::string str;
        std::getline(*in, str);
        if (argc == 2) {
            if (str.empty()) exit(0);
            std::cout << str << std::endl;
        }

        std::istringstream ss(str);
        std::string command;
        if (ss >> command) {
            if (command == "cr") {
                std::string name;
                if (!(ss >> name) || name.length() > DirectoryEntry::MAX_FILE_NAME_SIZE || name.length() == 0 ||
                    !ss.eof()) {
                    incorrectInput();
                } else {
                    try {
                        fs.createFile(name.c_str());
                        std::cout << "file " << name << " created" << std::endl;
                    } catch (const std::logic_error &le) {
                        std::cerr << le.what() << std::endl;
                    }
                }
            } else if (command == "de") {
                std::string name;
                if (!(ss >> name) || name.length() > DirectoryEntry::MAX_FILE_NAME_SIZE || name.length() == 0 ||
                    !ss.eof()) {
                    incorrectInput();
                } else {
                    try {
                        fs.destroyFile(name.c_str());
                        std::cout << "file " << name << " destroyed" << std::endl;
                    } catch (const std::logic_error &le) {
                        std::cerr << le.what() << std::endl;
                    }
                }
            } else if (command == "op") {
                std::string name;
                if (!(ss >> name) || !ss.eof()) {
                    incorrectInput();
                } else {
                    try {
                        int oft_index = fs.open(name.c_str());
                        std::cout << "file " << name << " opened, index = " << oft_index << std::endl;
                    } catch (const std::logic_error &le) {
                        std::cerr << le.what() << std::endl;
                    }
                }
            } else if (command == "cl") {
                int oft_index;
                if (!(ss >> oft_index) || !ss.eof()) {
                    incorrectInput();
                } else {
                    try {
                        fs.close(oft_index);
                        std::cout << "file " << oft_index << " closed" << std::endl;
                    } catch (const std::logic_error &le) {
                        std::cerr << le.what() << std::endl;
                    }
                }
            } else if (command == "rd") {
                int oft_index, count;
                if (!(ss >> oft_index, ss >> count) || !ss.eof()) {
                    incorrectInput();
                } else {
                    try {
                        char *char_to_read = new char[count];
                        int count_read = fs.read(oft_index, char_to_read, count);
                        std::cout << count_read << " bytes read: ";
                        for (int i = 0; i < count_read; i++) {
                            std::cout << char_to_read[i];
                        }
                        std::cout << std::endl;
                        delete[] char_to_read;
                    } catch (const std::logic_error &le) {
                        std::cerr << le.what() << std::endl;
                    }
                }
            } else if (command == "wr") {
                int oft_index, count;
                char c;
                if (!(ss >> oft_index, ss >> c, ss >> count) || !ss.eof()) {
                    incorrectInput();
                } else {
                    try {
                        char *char_to_write = new char[count];
                        std::fill(char_to_write, char_to_write + count, c);
                        int count_write = fs.write(oft_index, char_to_write, count);
                        std::cout << count_write << " bytes written" << std::endl;
                        delete[] char_to_write;
                    } catch (const std::logic_error &le) {
                        std::cerr << le.what() << std::endl;
                    }
                }
            } else if (command == "sk") {
                int oft_index, pos;
                if (!(ss >> oft_index, ss >> pos) || !ss.eof()) {
                    incorrectInput();
                } else {
                    try {
                        fs.lseek(oft_index, pos);
                        std::cout << "current position is " << pos << std::endl;
                    } catch (const std::logic_error &le) {
                        std::cerr << le.what() << std::endl;
                    }
                }
            } else if (command == "dr") {
                std::vector<std::string> filenames = fs.directory();
                if (filenames.empty()) {
                    std::cout << "directory is empty" << std::endl;
                }
                for (auto &filename : filenames) {
                    std::cout << filename << std::endl;
                }
            } else if (command == "in") {
                std::string filename;
                if (!(ss >> filename) || !ss.eof()) {
                    incorrectInput();
                } else {
                    std::string state = (fs.init(filename.c_str())) ? "disk restored" : "disk initialized";
                    std::cout << state << std::endl;
                }
            } else if (command == "sv") {
                std::string filename;
                if (!(ss >> filename) || !ss.eof()) {
                    incorrectInput();
                } else {
                    try {
                        fs.save(filename.c_str());
                        std::cout << "disk saved" << std::endl;
                    } catch (const std::logic_error &le) {
                        std::cerr << le.what() << std::endl;
                    }
                }
            } else {
                std::cout << "\'" << command << "\' is not recognized." << std::endl;
            }
        }
    }
}

void Shell::incorrectInput() {
    std::cout << "The input is incorrect." << std::endl;
}
