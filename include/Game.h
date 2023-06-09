#ifndef GAME_H
#define GAME_H
#include "Item.h"
#include <Initialize.h>
#include <UI.h>
#include <algorithm>
#include <cstring>
#include <curses.h>
#include <vector>

class Enemy;
class Player;
struct Location;
class Game {
public:
  static std::vector<std::string> ACTION_OPTIONS;
  static Player player;
  static std::vector<Item> INVENTORY;
  static std::vector<std::string> MAIN_MENU_OPTIONS;
  static std::vector<std::string> CHARACTERS;
  static int game_row, game_col, row_beg, col_beg;
  static std::vector<std::string> Enemy_list;
  static std::string S;
  static Enemy current_Enemy;
  static Item current_Item;
  static std::vector<Item> Item_List;
  enum GameState {
    Game_Title,
    Main_Menu,
    Character_Select,
    Rest,
    Play_Game,
    End_Screen,
    End_Game
  };
  static bool isEnemyLocation(Location L);
  static GameState game_state;
  static UI ui;
  static const char *Ascii_Title;
  static std::vector<Location> Obstacle_Locations;
  static bool isObstacleLocation(Location& L);
  static bool isPlayerLocation(Location L);
  static bool isGameObstacle(Location L);

  static void ExitGame()
  {
    Initialize::UnInitialize();
    std::exit(0);
  }
  static int Turn_Counter;
};
#endif // !GAME_H
