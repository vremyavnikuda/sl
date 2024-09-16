cmake_minimum_required(VERSION 3.29)
project(sl)  # Задаем имя проекта

set(CMAKE_CXX_STANDARD 17)  # Устанавливаем стандарт C++17

# Добавляем все необходимые исходные файлы для сборки
add_executable(sl
        main.cpp
        service/play_mp3.cpp
        service/get_playlist.h
        service/get_music_directory_from_config.h
        service/get_music_directory_from_config.cpp
        service/get_playlist.cpp
        service/play_playlist.cpp
        service/play_playlist.h
        ui/display_interface.cpp
        ui/display_interface.h
        global/globals.h

)

# Добавляем флаги для компоновки с библиотеками
target_link_libraries(sl mpg123 ao ncurses stdc++fs ui global)
