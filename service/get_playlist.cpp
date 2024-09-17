#include "get_playlist.h"
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <ostream>

namespace fs = std::filesystem;
/* get_playlist
Описание: Читает содержимое музыкальной директории и возвращает вектор путей к MP3-файлам.

Где используется: Вызов происходит в функции main, чтобы получить список всех MP3-файлов в указанной музыкальной директории.

Влияние на код: Если в музыкальной директории не будет найдено ни одного MP3-файла, программа завершится с сообщением об ошибке. Плейлист — это основной элемент для воспроизведения, и если он пуст, воспроизведение не начнется.
*/
std::vector<std::string> get_playlist(const std::string &directory)
{
    std::vector<std::string> playlist;
    try
    {
        // Итерируем по всем элементам в директории
        for (const auto &entry : fs::directory_iterator(directory))
        {
            // Проверяем, является ли текущий элемент обычным файлом и имеет ли он расширение .mp3
            if (entry.is_regular_file() && entry.path().extension() == ".mp3")
            {
                // Если условие выполняется, добавляем путь к файлу в вектор
                playlist.push_back(entry.path().string());
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error accessing the directory: " << e.what() << std::endl;
        // Возвращаем пустой вектор, если возникла ошибка при доступе к директории
        return {};
    }
    // Возвращаем вектор с путями к MP3-файлам
    return playlist;
}
