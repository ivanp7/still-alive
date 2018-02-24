#ifndef TSONG_H
#define TSONG_H

#include "output.h"

void PutChar(char ch, size_t x, size_t y)
{
    MoveCursorTo(x, y);

    char str[2] = {ch, '\0'};
    WriteString(str);
    RefreshScreen();
}

void PutText(const char* text, size_t x, size_t y, int number_of_chars = -1)
{
    size_t len = strlen(text);
    if (number_of_chars < 0)
        number_of_chars = len;

    for (int i = 0; i < number_of_chars; i++) {
        if (i < len)
            PutChar(text[i], x+i, y);
        else
            PutChar(' ', x+i, y);
    }
}

void TypeText(const char* text, size_t x, size_t y, size_t before_delay, size_t inter_delay,
        size_t after_delay, char cursorCharacter = ' ')
{
    if (cursorCharacter != ' ')
        PutChar(cursorCharacter, x, y);
    SleepFor(before_delay);

    MoveCursorTo(x, y);

    size_t c = 0;
    while (text[c] != '\0'){
        if (cursorCharacter != ' ')
            PutChar(cursorCharacter, x + c + 1, y);
        PutChar(text[c], x + c, y);

        if (text[c] != ' ')
            SleepFor(inter_delay);

        c++;
    }

    SleepFor(after_delay);

    if (cursorCharacter != ' ')
        PutChar(' ', x + c, y);
}

void ClearRectangle(size_t x, size_t y, size_t sx, size_t sy)
{
    for (size_t i = y; i < y + sy; i++){
        for (size_t j = x; j < x + sx; j++)
            PutChar(' ', j, i);
    }
}

#endif

