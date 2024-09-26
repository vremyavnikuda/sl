//
// Created by vremyavnikuda on 9/26/24.
//

#include "Music.h"

#include <fstream>
#include <global.h>
#include <iostream>
#include <mpg123.h>
#include <thread>
#include <ao/ao.h>
#include <filesystem>


/* play_playlist
Описание: Отвечает за воспроизведение всех треков в плейлисте по очереди. При завершении трека автоматически начинает воспроизведение следующего.

Где используется: Вызывается в отдельном потоке в функции main, чтобы начать воспроизведение плейлиста.

Влияние на код: Основная функция, управляющая воспроизведением треков. Она последовательно воспроизводит каждый трек из плейлиста, обновляя текущий индекс трека (track_index). При получении сигнала остановки или выхода (через флаги stop_playing и exit_flag) она завершает воспроизведение.
*/
void Music::play_playlist(const std::vector<std::string> &playlist, int &track_index) {
    try {
        while (!exit_flag) {
            // Сбрасываем флаг stop_playing перед началом трека
            stop_playing = false;
            std::cout << "Now playing: " << playlist[track_index] << std::endl;
            play_mp3(playlist[track_index]);

            // Проверяем флаг exit_flag после воспроизведения трека
            if (exit_flag)
                break;

            // Обновляем track_index на следующий трек в плейлисте
            track_index = (track_index + 1) % playlist.size();
        }
    } catch (const std::exception &e) {
        std::cerr << "An error occurred while playing the playlist: " << e.what() << std::endl;
    }
}

/* play_mp3
Описание: Воспроизводит один MP3-файл, используя библиотеки ao для аудиовывода и mpg123 для декодирования MP3.

Где используется: Используется в функции play_playlist для воспроизведения каждого трека по очереди.

Влияние на код: Эта функция отвечает за воспроизведение одного трека. Она проверяет флаги playing, stop_playing, и exit_flag для управления процессом воспроизведения. Если stop_playing или exit_flag активны, воспроизведение прекращается.

ТУТ ВООБЩЕ НИЧЕГО ТРОГАТЬ НЕЛЬЗЯ
*/
void Music::play_mp3(const std::string &file) {
    try {
        ao_initialize();
        mpg123_init();
        int driver = ao_default_driver_id();
        mpg123_handle *mh = mpg123_new(nullptr, nullptr);

        if (mpg123_open(mh, file.c_str()) != MPG123_OK) {
            throw std::runtime_error("Error opening MP3 file: " + file);
        }

        long rate;
        int channels, encoding;
        mpg123_getformat(mh, &rate, &channels, &encoding);

        ao_sample_format format;
        format.bits = mpg123_encsize(encoding) * 8;
        format.channels = channels;
        format.rate = rate;
        format.byte_format = AO_FMT_NATIVE;
        format.matrix = nullptr;

        ao_device *device = ao_open_live(driver, &format, nullptr);
        if (!device) {
            throw std::runtime_error("Error opening audio device.");
        }

        size_t done;
        size_t buffer_size = mpg123_outblock(mh);
        auto *buffer = new unsigned char[buffer_size];

        while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK && !stop_playing && !exit_flag) {
            if (playing) {
                ao_play(device, reinterpret_cast<char *>(buffer), done);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        delete[] buffer;
        ao_close(device);
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        ao_shutdown();
    } catch (const std::exception &e) {
        std::cerr << "An error occurred while playing the MP3 file: " << e.what() << std::endl;
    }
}

/*
get_music_directory_from_config
Описание: Эта функция читает файл конфигурации и извлекает путь к музыкальной директории.

Где используется: Вызов происходит в функции main, чтобы получить путь к музыкальной директории из файла конфигурации (sl.conf).

Влияние на код: Если путь не будет найден или файл конфигурации не удастся открыть, программа не сможет продолжить работу, так как плейлист не будет загружен. В случае ошибки возвращает пустую строку, что приведет к завершению программы с ошибкой.
*/
std::string Music::get_music_directory_from_config(const std::string &config_file) {
    try {
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
            throw std::runtime_error("Could not open config file: " + config_file);
        }

        return music_directory;
    } catch (const std::exception &e) {
        std::cerr << "An error occurred while getting music directory from config: " << e.what() << std::endl;
        return "";
    }
}

/* get_playlist
Описание: Читает содержимое музыкальной директории и возвращает вектор путей к MP3-файлам.

Где используется: Вызов происходит в функции main, чтобы получить список всех MP3-файлов в указанной музыкальной директории.

Влияние на код: Если в музыкальной директории не будет найдено ни одного MP3-файла, программа завершится с сообщением об ошибке. Плейлист — это основной элемент для воспроизведения, и если он пуст, воспроизведение не начнется.
*/
std::vector<std::string> Music::get_playlist(const std::string &directory) {
    std::vector<std::string> playlist;
    try {
        // Итерируем по всем элементам в директории
        for (const auto &entry : std::filesystem::directory_iterator(directory)) {
            // Проверяем, является ли текущий элемент обычным файлом и имеет ли он расширение .mp3
            if (entry.is_regular_file() && entry.path().extension() == ".mp3") {
                // Если условие выполняется, добавляем путь к файлу в вектор
                playlist.push_back(entry.path().string());
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error accessing the directory: " << e.what() << std::endl;
        // Возвращаем пустой вектор, если возникла ошибка при доступе к директории
        return {};
    }
    // Возвращаем вектор с путями к MP3-файлам
    return playlist;
}
