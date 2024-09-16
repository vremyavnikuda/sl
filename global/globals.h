#ifndef GLOBALS_H
#define GLOBALS_H

#include <atomic>

// Глобальные переменные
extern std::atomic<bool> playing;
extern std::atomic<bool> stop_playing;
extern std::atomic<bool> exit_flag;

#endif // GLOBALS_H
