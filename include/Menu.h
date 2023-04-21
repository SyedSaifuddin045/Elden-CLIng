#ifndef MENU_H
#define MENU_H

#include <menu.h>
#include <ncurses.h>
#include <vector>
#include <string>

class Menu {
private:
  MENU* menu;
  WINDOW* window;
  std::vector<ITEM*> items;

public:
  Menu() : menu(nullptr), window(nullptr) {}

  void Create(WINDOW* win, int y, int x, int height, int width, std::vector<std::string> options) {
    if (options.empty()) {
      return;
    }

    window = derwin(win, height, width, y, x);
    keypad(window, true);

    for (auto& option : options) {
      items.push_back(new_item(option.c_str(), ""));
    }
    items.push_back(nullptr);

    menu = new_menu(items.data());
    set_menu_win(menu, window);
    set_menu_sub(menu, derwin(window, height, width - 2, 0, 1));
    set_menu_format(menu, height, 1);
    post_menu(menu);

    wrefresh(window);
  }
  std::string Handle_Input()
  {
    int c;
    while((c=wgetch(window)) != KEY_F(1))
    {
    switch (c) {
      case KEY_DOWN:
        menu_driver(menu, REQ_DOWN_ITEM);
        wrefresh(window);
        break;
      case KEY_UP:
        menu_driver(menu, REQ_UP_ITEM);
        wrefresh(window);
        break;
      case 10:
      case KEY_ENTER:
        int selected_index = GetSelectedIndex();
        mvprintw(0, 0, "Select Item = %s",item_name(items[selected_index]));
        wrefresh(window);
        return item_name(items[selected_index]);
        break;
    }
    }
    return item_name(items[0]);
  }
  void Destroy() {
    if (menu != nullptr) {
      unpost_menu(menu);
      free_menu(menu);
      menu = nullptr;
    }

    for (auto& item : items) {
      free_item(item);
    }
    items.clear();

    if (window != nullptr) {
      werase(window);
      wrefresh(window);
      delwin(window);
      window = nullptr;
    }
  }

  void Refresh() {
    if (window != nullptr) {
      wrefresh(window);
    }
  }

  int GetSelectedIndex() {
    if (menu == nullptr) {
      return -1;
    }

    ITEM* item = current_item(menu);
    if (item == nullptr) {
      return -1;
    }

    int index = item_index(item);
    return index;
  }

  void Set_Menu_Options_On(Menu_Options M_Option)
  {
    menu_opts_on(menu,M_Option);
  }
  void Set_Menu_Options_Off(Menu_Options M_Option)
  {
    menu_opts_off(menu,M_Option);
  }
  MENU* getMENU(){return menu;}
};

#endif // MENU_H
