#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include <Initialize.h>
#include <UI.h>
#include <algorithm>
#include <cstring>
#include <curses.h>

class Game {
public:
  static std::vector<std::string> ACTION_OPTIONS;
  static Player player;
  struct Location {
    int x, y;
  };
  static std::vector<std::string> INVENTORY;
  static std::vector<std::string> MAIN_MENU_OPTIONS;
  static std::vector<std::string> CHARACTERS;
  static int game_row, game_col, row_beg, col_beg;
  enum GameState {
    Game_Title,
    Main_Menu,
    Character_Select,
    Play_Game,
    End_Screen,
    End_Game
  };
  static GameState game_state;
  static UI ui;
  static const char *Ascii_Title;
  static std::vector<Location> Obstacle_Locations;
  static bool isObstacleLocation(Game::Location L) {
    for (Location OL : Game::Obstacle_Locations) 
    {
      if (L.x == OL.x && L.y == OL.y)
      {
        return true;
      }
    }
    return false;
  }
  static void ExitGame()
  {
    Initialize::UnInitialize();
    std::exit(0);
  }
};
#endif // !GAME_H
