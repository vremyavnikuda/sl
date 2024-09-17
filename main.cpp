#include <ncurses.h>
#include <thread>
#include <atomic>
#include <iostream>
#include <ostream>
#include <vector>
#include "service/get_music_directory_from_config.h"
#include "service/get_playlist.h"
#include "service/play_playlist.h"
#include "ui/display_interface.h"
#include "global/globals.h"

// Определение глобальных переменных
std::atomic<bool> playing{true};
std::atomic<bool> stop_playing{false};
std::atomic<bool> exit_flag{false};

/* MAIN
Описание: Главная функция программы. Загружает конфигурацию, получает плейлист, запускает воспроизведение треков в отдельном потоке и обрабатывает пользовательский ввод.

Где используется: Запускается при запуске программы.

Влияние на код: Главная функция управляет всем процессом программы, начиная с загрузки конфигурации и плейлиста, и заканчивая воспроизведением треков. Она также обрабатывает пользовательский ввод, что позволяет переключать треки, ставить воспроизведение на паузу или выходить из программы.
 */
int main()
{
    try
    {
        // Путь к файлу конфигурации
        const std::string config_file = "sl.conf";

        // Чтение директории с музыкой из конфигурационного файла
        const std::string music_directory = get_music_directory_from_config(config_file);

        if (music_directory.empty())
        {
            throw std::runtime_error("No music directory specified in config file.");
        }

        // Получение плейлиста
        std::vector<std::string> playlist = get_playlist(music_directory);

        if (playlist.empty())
        {
            throw std::runtime_error("No MP3 files found in directory: " + music_directory);
        }

        int track_index = 0;

        // Запуск плейлиста в отдельном потоке
        std::thread player_thread(play_playlist, std::ref(playlist), std::ref(track_index));

        // Инициализация ncurses
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        // Включаем неблокирующий режим для getch()
        nodelay(stdscr, TRUE);

        bool running = true;

        // Главный цикл для обработки пользовательского ввода
        while (running)
        {
            const int ch = getch();

            // Обработка клавиш
            switch (ch)
            {
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
                track_index = (track_index - 1 + playlist.size()) % playlist.size();
                break;

            // Стрелка вниз: выбор следующего трека
            case KEY_DOWN:
                track_index = (track_index + 1) % playlist.size();
                break;

            // Enter: воспроизвести выбранный трек
            case '\n':
                stop_playing = true;
                // Воспроизведение выбранного трека
                playing = true;
                break;

            default:
                break;
            }

            // timesleep low thread
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Отображение интерфейса
            display_interface(playlist, track_index);
        }

        // Завершаем работу ncurses
        endwin();

        // Ожидаем завершения потоков -> не блокируемые
        player_thread.join();

        std::cout << "All tracks finished." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
