//
// Created by vremyavnikuda on 9/26/24.
//

#ifndef MUSIC_H
#define MUSIC_H
#include <string>
#include <vector>

class Music {
public:
    static void play_playlist(const std::vector<std::string> &playlist, int &track_index);

    static std::string get_music_directory_from_config(const std::string &config_file_path);

    static void play_mp3(const std::string &file);

    static std::vector<std::string> get_playlist(const std::string &directory);
};


#endif //MUSIC_H
