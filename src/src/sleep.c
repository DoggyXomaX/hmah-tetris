#include "sleep.h"

#ifdef _WIN32
    #include <windows.h>
    void sleep_ms(int milliseconds) {
        Sleep(milliseconds);
    }
#elif __unix
    #define _POSIX_C_SOURCE 199309L // or greater
    #include <time.h>
    void sleep_ms(int milliseconds) {
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = milliseconds % 1000 * 1000;
        nanosleep(&ts, (void*)0);
    }
#else
    error "Unknown system :("
#endif