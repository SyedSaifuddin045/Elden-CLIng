#include <menu.h>
#include <ncurses.h>
#include <string>
#include <vector>

#ifndef MENU_H
#define MENU_H
class Menu {
  MENU *menu;
  ITEM **items;
public:
  Menu():items(NULL),menu(nullptr){};
  MENU* getMENU()const { return menu;}
  void setMENU(MENU* m) { menu = m; }
};
#endif // !MENU_H
