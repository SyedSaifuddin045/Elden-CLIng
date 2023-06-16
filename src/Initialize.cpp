#include <Initialize.h>
#include <curses.h>

Initialize::Initialize()
{
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2,COLOR_CYAN,COLOR_BLACK);
  init_pair(3, COLOR_BLACK, COLOR_CYAN);
  init_pair(4,COLOR_MAGENTA,COLOR_BLACK);
  init_pair(5,COLOR_GREEN,COLOR_BLACK);
  init_pair(6,COLOR_RED,COLOR_BLACK);
}
void Initialize::UnInitialize()
{
  endwin();
}
