#ifndef __TETRIS_H__
#define __TETRIS_H__

#include <stdbool.h>

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600
#define WINDOW_HEADER "Хмах тетрис!"

extern bool g_IsRunning;

int App_Init(int argc, char* argv[]);

#endif // __TETRIS_H__