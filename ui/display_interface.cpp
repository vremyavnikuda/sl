#include <ncurses.h>
#include <vector>
#include <string>

void display_interface(const std::vector<std::string> &playlist, const int &track_index)
{
    int row, col;
    clear(); // Очищаем экран

    // Получаем размер окна терминала
    getmaxyx(stdscr, row, col);

    // Отображаем текущую песню в верхней строке
    mvprintw(0, 0, "Now playing: %s", playlist[track_index].c_str());

    // Отображаем список треков
    for (int i = 0; i < playlist.size() && i < row - 1; ++i)
    {
        if (i == track_index)
        {
            // Подсвечиваем текущий трек
            attron(A_REVERSE);
            mvprintw(i + 1, 0, "%s", playlist[i].c_str());
            attroff(A_REVERSE);
        }
        else
        {
            mvprintw(i + 1, 0, "%s", playlist[i].c_str());
        }
    }

    refresh(); // Обновляем экран
}
