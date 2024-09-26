cmake_minimum_required(VERSION 3.29)
project(sll)

set(CMAKE_CXX_STANDARD 17)  # или выше

include_directories(
        ${CMAKE_SOURCE_DIR}/global
        ${CMAKE_SOURCE_DIR}/service
        ${CMAKE_SOURCE_DIR}/ui
)

# Проверка наличия необходимых библиотек
find_package(PkgConfig REQUIRED)

pkg_check_modules(AO REQUIRED libao)
if (NOT AO_FOUND)
    message(FATAL_ERROR "libao not found")
endif()

pkg_check_modules(MPG123 REQUIRED mpg123)
if (NOT MPG123_FOUND)
    message(FATAL_ERROR "mpg123 not found")
endif()

find_package(Curses REQUIRED)  # для ncurses

add_executable(sll main.cpp
        service/Music.cpp
        global/global.h
        ui/Ui.cpp
        ui/Ui.h
)

target_link_libraries(sll PRIVATE Curses::Curses ${AO_LIBRARIES} ${MPG123_LIBRARIES})
