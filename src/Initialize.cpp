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
}
void Initialize::UnInitialize()
{
  endwin();
}
