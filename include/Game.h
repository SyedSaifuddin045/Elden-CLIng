#include <Initialize.h>
#include <UI.h>
#include <string>
#include <vector>

class Game {
public:
  static std::vector<std::string> ACTION_OPTIONS;

  static std::vector<std::string> INVENTORY;
  enum GameState { Main_Menu, Character_Select, Play_Game, End_Screen };
  static GameState game_state;
  static UI ui;
};
