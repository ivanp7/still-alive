// Platform-dependent functions

#include "io.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

void PlayMusic(const char* file)
{
    char command[256]; command[0] = '\0';

    strcat(command, "mpv --no-terminal ");
    strcat(command, file);
    strcat(command, " &");

    system(command);
}

void SleepFor(int time_ms)
{
    timespec tm = {time_ms/1000, (time_ms%1000) * 1000000}, rem;
    nanosleep(&tm, &rem);
}

void InitializeScreen()
{
  initscr();
  start_color();
}

void ClearScreen()
{
  clear();
}

void RefreshScreen()
{
  refresh();
}

void ReleaseScreen()
{
  endwin();
}

void HideCursor()
{
  curs_set(0);
}

void ShowCursor()
{
  curs_set(1);
}

void InitColorPair(int pairNo, int foregroundColor, int backgroundColor)
{
  init_pair(pairNo, foregroundColor, backgroundColor);
}

void SetWriteColorPair(int pairNo)
{
  attrset(COLOR_PAIR(pairNo));
}

void SetClearColorPair(int pairNo)
{
  bkgdset(COLOR_PAIR(pairNo));
}

int GetScreenSizeX()
{
  return getmaxx(stdscr);
}

int GetScreenSizeY()
{
  return getmaxy(stdscr);
}

void MoveCursorTo(int x, int y)
{
  move(y, x);
}

void WriteString(const char* str)
{
  printw(str);
}

int WaitForKey()
{
    return getch();
}

