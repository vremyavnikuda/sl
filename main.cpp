#include <ncurses.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <vector>
#include "service/get_music_directory_from_config.h"
#include "service/get_playlist.h"
#include "service/play_playlist.h"
#include "ui/display_interface.h"
#include "global/globals.h"
#include "service/handle_input.h"

// Определение глобальных переменных
std::atomic<bool> playing{true};
std::atomic<bool> stop_playing{false};
std::atomic<bool> exit_flag{false};

int main()
{
    // Путь к файлу конфигурации
    const std::string config_file = "sl.conf";

    // Чтение директории с музыкой из конфигурационного файла
    const std::string music_directory = get_music_directory_from_config(config_file);

    if (music_directory.empty())
    {
        std::cerr << "No music directory specified in config file." << std::endl;
        return 1;
    }

    // Получение плейлиста
    std::vector<std::string> playlist = get_playlist(music_directory);

    if (playlist.empty())
    {
        std::cerr << "No MP3 files found in directory: " << music_directory << std::endl;
        return 1;
    }

    int track_index = 0;

    // Запуск плейлиста в отдельном потоке
    std::thread player_thread(play_playlist, std::ref(playlist), std::ref(track_index));

    // Инициализация ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Включаем неблокирующий режим для getch()

    bool running = true;

    // Главный цикл для обработки пользовательского ввода
    while (running)
    {
        const int ch = getch();

        // Обработка клавиш
        switch (ch)
        {
        case 'n': // Следующий трек
            stop_playing = true;
            track_index = (track_index + 1) % playlist.size();
            break;

        case 'r': // Предыдущий трек
            stop_playing = true;
            track_index = (track_index - 1 + playlist.size()) % playlist.size();
            break;

        case ' ': // Пауза/Продолжить
            playing = !playing;
            break;

        case 'q': // Выйти из программы
            running = false;
            exit_flag = true;
            stop_playing = true;
            break;

        case KEY_UP: // Стрелка вверх: выбор предыдущего трека
            track_index = (track_index - 1 + playlist.size()) % playlist.size();
            break;

        case KEY_DOWN: // Стрелка вниз: выбор следующего трека
            track_index = (track_index + 1) % playlist.size();
            break;

        case '\n': // Enter: воспроизвести выбранный трек
            stop_playing = true;
            break;

        default:
            break;
        }

        // Ожидание для снижения нагрузки на процессор
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Отображение интерфейса
        display_interface(playlist, track_index);
    }

    // Завершаем работу ncurses
    endwin();

    // Ожидаем завершения потоков
    player_thread.join();

    std::cout << "All tracks finished." << std::endl;

    return 0;
}
