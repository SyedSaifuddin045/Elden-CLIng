#ifndef WINDOW_H
#define WINDOW_H

#include <Menu.h>
#include <ncurses.h>
#include <string>

class Window {
private:
  WINDOW *window;
  int width, height, pos_x, pos_y;
  Menu menu;

public:
  Window() : width(COLS), height(LINES), pos_x(0), pos_y(0) {
    window = newwin(height, width, pos_y, pos_x);
    keypad(window, true);
  }
  Window(int _Height, int _Width, int _PosY, int _PosX)
      : height(_Height), width(_Width), pos_x(_PosX), pos_y(_PosY) {
    window = newwin(height, width, pos_y, pos_x);
    keypad(window, true);
  }
  void Box(chtype Vertical, chtype Horizontal) {
    box(window, Vertical, Horizontal);
  }
  void Refresh() { wrefresh(window); }
  void Print_Window_Title(std::string &Title) {
    int start_y, start_x, max_y, max_x; 
    int title_length = Title.size();

    getbegyx(window, start_y, start_x);
    getmaxyx(window, max_y, max_x);

    wattron(window, COLOR_PAIR(1));

    mvwaddch(window, 2, 0, ACS_LTEE);
    mvwhline(window, 2, 1, ACS_HLINE, max_x - 2);
    mvwaddch(window, 2, max_x - 1, ACS_RTEE);

    wattroff(window, COLOR_PAIR(1));

    wattron(window, COLOR_PAIR(2) | A_REVERSE);

    mvwprintw(window, 1, ((max_x - title_length) / 2), "%s", Title.c_str());

    wattroff(window, COLOR_PAIR(2) | A_REVERSE);

    wrefresh(window);
  }
  void DestroyWindow() {
    if (window != nullptr) {
      wclear(window);
      wrefresh(window);
      delwin(window);
      refresh();
      window = nullptr;
    }
  }
  void DestroyMenu() { menu.Destroy(); }
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  int getPosX() const { return pos_x; }
  int getPosY() const { return pos_y; }
  WINDOW *getWindow() const { return window; }
  Menu getMenu() const { return menu; }
  void setMenu(Menu M) { menu = M; }
};
#endif //  WINDOW_H
