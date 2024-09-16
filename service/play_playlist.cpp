#include "play_playlist.h"
#include "globals.h"
#include <iostream>

#include "play_mp3.h"

void play_playlist(const std::vector<std::string>& playlist, int& track_index) {
    while (!exit_flag) {
        // Сбрасываем stop_playing перед началом трека
        stop_playing = false;
        std::cout << "Now playing: " << playlist[track_index] << std::endl;
        play_mp3(playlist[track_index]);

        if (exit_flag) break;

        track_index = (track_index + 1) % playlist.size();
    }
}
