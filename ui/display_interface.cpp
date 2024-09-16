#include <ncurses.h>
#include <vector>
#include <string>

void display_interface(const std::vector<std::string> &playlist, const int &track_index) {
    int row, col;
    // Очищаем экран
    clear();

    // Получаем размер окна терминала
    getmaxyx(stdscr, row, col);

    //ширина рамки по горизонтали
    int playlist_width = col - 2;
    mvprintw(0, 0, "+");
    mvprintw(0, playlist_width + 1, "+");

    for (int i = 1; i < row - 1; ++i) {
        mvprintw(i, 0, "|");
        mvprintw(i, playlist_width + 1, "|");
    }

    mvprintw(row - 1, 0, "+");
    mvprintw(row - 1, playlist_width + 1, "|");

    for (int i = 1; i < playlist_width + 1; ++i) {
        mvprintw(0, i, "-");
        mvprintw(row - 1, i, "-");
    }

    // Отображаем текущую песню в верхней строке
    mvprintw(0, 2, "Now playing: %s", playlist[track_index].c_str());

    // Отображаем список треков
    for (int i = 0; i < playlist.size() && i < row - 3; ++i) {
        if (i == track_index) {
            // Подсвечиваем текущий трек
            attron(A_REVERSE);
            // Сдвигаем на 2 символа вправо для рамки
            mvprintw(i + 2, 2, "%s", playlist[i].c_str());
            attroff(A_REVERSE);
        } else {
            // Сдвигаем на 2 символа вправо для рамки
            mvprintw(i + 2, 2, "%s", playlist[i].c_str());
        }
    }
    // Обновляем экран
    refresh();
}
