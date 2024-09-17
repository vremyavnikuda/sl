#include "get_music_directory_from_config.h"
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

/*
get_music_directory_from_config
Описание: Эта функция читает файл конфигурации и извлекает путь к музыкальной директории.

Где используется: Вызов происходит в функции main, чтобы получить путь к музыкальной директории из файла конфигурации (sl.conf).

Влияние на код: Если путь не будет найден или файл конфигурации не удастся открыть, программа не сможет продолжить работу, так как плейлист не будет загружен. В случае ошибки возвращает пустую строку, что приведет к завершению программы с ошибкой.
*/
std::string get_music_directory_from_config(const std::string &config_file)
{
    try
    {
        std::ifstream config(config_file);
        std::string line;
        std::string music_directory;

        if (config.is_open())
        {
            while (std::getline(config, line))
            {
                if (line.find("music_directory=") == 0)
                {
                    music_directory = line.substr(std::string("music_directory=").length());
                    break;
                }
            }
            config.close();
        }
        else
        {
            throw std::runtime_error("Could not open config file: " + config_file);
        }

        return music_directory;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred while getting music directory from config: " << e.what() << std::endl;
        return "";
    }
}
