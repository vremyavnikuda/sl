#include "get_music_directory_from_config.h"
#include <fstream>
#include <iostream>
#include <string>

std::string get_music_directory_from_config(const std::string& config_file) {
    std::ifstream config(config_file);
    std::string line;
    std::string music_directory;

    if (config.is_open()) {
        while (std::getline(config, line)) {
            if (line.find("music_directory=") == 0) {
                music_directory = line.substr(std::string("music_directory=").length());
                break;
            }
        }
        config.close();
    } else {
        std::cerr << "Could not open config file: " << config_file << std::endl;
    }

    return music_directory;
}
