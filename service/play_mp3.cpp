#include "play_mp3.h"
#include "globals.h"
#include <ao/ao.h>
#include <mpg123.h>
#include <iostream>
#include <thread>
#include <ostream>

/* play_mp3
Описание: Воспроизводит один MP3-файл, используя библиотеки ao для аудиовывода и mpg123 для декодирования MP3.

Где используется: Используется в функции play_playlist для воспроизведения каждого трека по очереди.

Влияние на код: Эта функция отвечает за воспроизведение одного трека. Она проверяет флаги playing, stop_playing, и exit_flag для управления процессом воспроизведения. Если stop_playing или exit_flag активны, воспроизведение прекращается.

ТУТ ВООБЩЕ НИЧЕГО ТРОГАТЬ НЕЛЬЗЯ
*/
void play_mp3(const std::string &file)
{
    try
    {
        ao_initialize();
        mpg123_init();
        int driver = ao_default_driver_id();
        mpg123_handle *mh = mpg123_new(nullptr, nullptr);

        if (mpg123_open(mh, file.c_str()) != MPG123_OK)
        {
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
        if (!device)
        {
            throw std::runtime_error("Error opening audio device.");
        }

        size_t done;
        size_t buffer_size = mpg123_outblock(mh);
        auto *buffer = new unsigned char[buffer_size];

        while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK && !stop_playing && !exit_flag)
        {
            if (playing)
            {
                ao_play(device, reinterpret_cast<char *>(buffer), done);
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        delete[] buffer;
        ao_close(device);
        mpg123_close(mh);
        mpg123_delete(mh);
        mpg123_exit();
        ao_shutdown();
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred while playing the MP3 file: " << e.what() << std::endl;
    }
}
