#include "play_playlist.h"
#include "globals.h"
#include <iostream>
#include <ostream>

#include "play_mp3.h"

/* play_playlist
Описание: Отвечает за воспроизведение всех треков в плейлисте по очереди. При завершении трека автоматически начинает воспроизведение следующего.

Где используется: Вызывается в отдельном потоке в функции main, чтобы начать воспроизведение плейлиста.

Влияние на код: Основная функция, управляющая воспроизведением треков. Она последовательно воспроизводит каждый трек из плейлиста, обновляя текущий индекс трека (track_index). При получении сигнала остановки или выхода (через флаги stop_playing и exit_flag) она завершает воспроизведение.
*/
void play_playlist(const std::vector<std::string> &playlist, int &track_index)
{
    try
    {
        while (!exit_flag)
        {
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
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred while playing the playlist: " << e.what() << std::endl;
    }
}
