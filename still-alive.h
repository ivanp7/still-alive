// Portal 1 Ending Credits remake
// Written by Ivan Podmazov, 2011
// Remade for Linux in 2016
// Rewritten in 2018

// GLaDOS 3.11 - [Still Alive]

#pragma once

#include <stddef.h>

#define DEFAULT_INTER_DELAY 100

void WriteLine(const char* text, size_t after_delay, size_t inter_delay = DEFAULT_INTER_DELAY);
void DrawPicture(const char* image, int X = -1, int Y = -1);
void ClearArea();
int Song();

