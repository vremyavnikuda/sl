#include <ncurses.h>
#include <vector>
#include <string>
#include <iostream>

/*display_interface

Описание: Отображает интерфейс на экране с помощью библиотеки ncurses, включая текущий трек, который воспроизводится, и список треков.
-> add : форматирование отображения песни ,а не пути для файла (.mp3) в плейлисте

Где используется: Вызывается в основном цикле программы внутри функции main, чтобы обновить интерфейс на экране.

Влияние на код: Функция обновляет отображение текущего трека и позволяет пользователю видеть список воспроизводимых файлов. Обновление интерфейса происходит в цикле с частотой примерно каждые 100 миллисекунд, что поддерживает интерфейс интерактивным.
*/
void display_interface(const std::vector<std::string> &playlist, const int &track_index)
{
    try
    {
        int row, col;
        // Очищаем экран
        clear();

        // Получаем размер окна терминала
        getmaxyx(stdscr, row, col);

        // ширина рамки по горизонтали
        int playlist_width = col - 2;
        mvprintw(0, 0, "+");
        mvprintw(0, playlist_width + 1, "+");

        for (int i = 1; i < row - 1; ++i)
        {
            mvprintw(i, 0, "|");
            mvprintw(i, playlist_width + 1, "|");
        }

        mvprintw(row - 1, 0, "+");
        mvprintw(row - 1, playlist_width + 1, "|");

        for (int i = 1; i < playlist_width + 1; ++i)
        {
            mvprintw(0, i, "-");
            mvprintw(row - 1, i, "-");
        }

        // Отображаем текущую песню в верхней строке
        mvprintw(0, 2, "Now playing: %s", playlist[track_index].substr(playlist[track_index].find_last_of("/\\") + 1).c_str());

        // Отображаем список треков
        for (std::size_t i = 0; i < playlist.size() && i < row - 3; ++i)
        {
            if (i == track_index)
            {
                // Подсвечиваем текущий трек
                attron(A_REVERSE);
                // Сдвигаем на 2 символа вправо для рамки
                mvprintw(i + 2, 2, "%s", playlist[i].substr(playlist[i].find_last_of("/\\") + 1).c_str());
                attroff(A_REVERSE);
            }
            else
            {
                // Сдвигаем на 2 символа вправо для рамки
                mvprintw(i + 2, 2, "%s", playlist[i].substr(playlist[i].find_last_of("/\\") + 1).c_str());
            }
        }
        // Обновляем экрн
        refresh();
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred while displaying the interface: " << e.what() << std::endl;
    }
}
