//
// Created by vremyavnikuda on 9/26/24.
//

#ifndef GLOBAL_H
#define GLOBAL_H
#include <atomic>
// Глобальные переменные
extern std::atomic<bool> playing;
extern std::atomic<bool> stop_playing;
extern std::atomic<bool> exit_flag;
#endif //GLOBAL_H
