#include "handle_input.h"

#include <chrono>
#include <ncurses.h>
#include <string>
#include <thread>
#include <vector>
#include "globals.h"

void handle_input(bool& running, int& track_index, const std::vector<std::string>& playlist) {
    int selected_track = track_index;

    while (running) {
        const int ch = getch();

        switch (ch) {
            // Следующий трек
            case 'n':
                stop_playing = true;
                track_index = (track_index + 1) % playlist.size();
                break;

            // Предыдущий трек
            case 'r':
                stop_playing = true;
                track_index = (track_index - 1 + playlist.size()) % playlist.size();
                break;

            // Пауза/Продолжить
            case ' ':
                playing = !playing;
                break;

            // Выйти из программы
            case 'q':
                running = false;
                exit_flag = true;
                stop_playing = true;
                break;

            // Стрелка вверх: выбор предыдущего трека
            case KEY_UP:
                selected_track = (selected_track - 1 + playlist.size()) % playlist.size();
                break;

            // Стрелка вниз: выбор следующего трека
            case KEY_DOWN:
                selected_track = (selected_track + 1) % playlist.size();
                break;

            // Enter: воспроизвести выбранный трек
            case '\n':
                stop_playing = true;
                track_index = selected_track;
                break;

            default:
                break;
        }

        // Сброс флага stop_playing после небольшой задержки, чтобы текущий трек мог корректно завершиться.
        if (stop_playing) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            stop_playing = false; // Reset stop_playing after a delay
        }

        // Задержка для уменьшения нагрузки на процессор
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
