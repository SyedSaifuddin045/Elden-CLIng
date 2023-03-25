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
}

void Initialize::UnInitialize()
{
  endwin();
}
