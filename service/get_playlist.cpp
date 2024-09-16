#include "get_playlist.h"
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

std::vector<std::string> get_playlist(const std::string& directory) {
    std::vector<std::string> playlist;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".mp3") {
            playlist.push_back(entry.path().string());
        }
    }
    return playlist;
}
