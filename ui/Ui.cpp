//
// Created by vremyavnikuda on 9/26/24.
//

#include "Ui.h"
#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>


/*display_interface

Описание: Отображает интерфейс на экране с помощью библиотеки ncurses, включая
текущий трек, который воспроизводится, и список треков.
-> add : форматирование отображения песни ,а не пути для файла (.mp3) в
плейлисте

Где используется: Вызывается в основном цикле программы внутри функции main,
чтобы обновить интерфейс на экране.

Влияние на код: Функция обновляет отображение текущего трека и позволяет
пользователю видеть список воспроизводимых файлов. Обновление интерфейса
происходит в цикле с частотой примерно каждые 100 миллисекунд, что поддерживает
интерфейс интерактивным.
*/

/*TODO: создать 2 блока в одном будет содержаться плейлист в другом будет
отображаться визуальное сопровождение проигрываемой песни
*/

// Функция для отображения аудиоспектра
// TODO:сделать привязку к
void Ui::display_audio_spectrum(int spectrum_height, int col) {
    int spectrum_width = col - 2; // ширина спектра

    // Генерация и отображение столбцов аудиоспектра
    for (int i = 1; i < spectrum_width; ++i) {
        int column_height =
                rand() % spectrum_height + 1; // случайная высота столбца

        // Рисуем столбцы снизу вверх
        for (int j = spectrum_height; j > spectrum_height - column_height;
             --j) {
            mvprintw(j, i, "|");
        }
    }
}


void Ui::display_interface(const std::vector<std::string> &playlist, int &track_index) {
    Ui uiInstance;
    try {
        int row, col;
        // Очищаем экран
        clear();

        // Инициализация цветов
        start_color();
        // Цвет заголовка "Now playing"
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        // Цвет активного трека
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        // Цвет неактивных треков
        init_pair(3, COLOR_WHITE, COLOR_BLACK);

        // Получаем размер окна терминала
        getmaxyx(stdscr, row, col);

        // Верхняя часть для аудиоспектра
        int split_row = row / 2; // Половина экрана для разделения

        // ----------- Аудиоспектр в верхней части ------------
        // Рисуем рамку вокруг верхней части
        mvprintw(0, 0, "+");
        mvprintw(0, col - 1, "+");
        for (int i = 1; i < split_row; ++i) {
            mvprintw(i, 0, "|");
            mvprintw(i, col - 1, "|");
        }
        mvprintw(split_row, 0, "+");
        mvprintw(split_row, col - 1, "+");
        for (int i = 1; i < col - 1; ++i) {
            mvprintw(0, i, "-");
            mvprintw(split_row, i, "-");
        }

        // Отображаем аудиоспектр в верхней части
        uiInstance.display_audio_spectrum(split_row - 2, col);

        // ----------- Плейлист и текущий трек в нижней части ------------
        // Нижняя часть для отображения информации о треках
        mvprintw(split_row + 1, 0, "+");
        mvprintw(split_row + 1, col - 1, "+");
        for (int i = split_row + 2; i < row - 1; ++i) {
            mvprintw(i, 0, "|");
            mvprintw(i, col - 1, "|");
        }
        mvprintw(row - 1, 0, "+");
        mvprintw(row - 1, col - 1, "+");
        for (int i = 1; i < col - 1; ++i) {
            mvprintw(split_row + 1, i, "-");
            mvprintw(row - 1, i, "-");
        }

        // Отображаем текущую песню в нижней части
        mvprintw(split_row + 2, 2, "Now playing:");
        attron(COLOR_PAIR(1));
        mvprintw(split_row + 3, 2, "%s",
                 playlist[track_index]
                 .substr(playlist[track_index].find_last_of("/\\") + 1)
                 .c_str());
        attroff(COLOR_PAIR(1));

        // Отображаем список треков под текущей песней
        mvprintw(split_row + 5, 2, "Playlist:");
        for (std::size_t i = 0; i < playlist.size() && i < (row - split_row - 6); ++i) {
            if (i == track_index) {
                // Подсвечиваем текущий трек
                attron(A_REVERSE);
                attron(COLOR_PAIR(2));
                mvprintw(split_row + 6 + i, 2, "%s",
                         playlist[i]
                         .substr(playlist[i].find_last_of("/\\") + 1)
                         .c_str());
                attroff(COLOR_PAIR(2));
                attroff(A_REVERSE);
            } else {
                mvprintw(split_row + 6 + i, 2, "%s",
                         playlist[i]
                         .substr(playlist[i].find_last_of("/\\") + 1)
                         .c_str());
            }
        }

        // Обновляем экран
        refresh();
    } catch (const std::exception &e) {
        std::cerr << "An error occurred while displaying the interface: "
                << e.what() << std::endl;
    }
}

// навигационная панель управления
// TODO: пока не знаю какие функции будет выполнять данный метод
//void interface_display() {}
