#include <Initialize.h>
#include <UI.h>

class Game {
public:
  static std::vector<std::string> ACTION_OPTIONS;

  static std::vector<std::string> INVENTORY;
  static std::vector<std::string> MAIN_MENU_OPTIONS;
  static std::vector<std::string> CHARACTERS;
  enum GameState { Main_Menu, Character_Select, Play_Game, End_Screen };
  static GameState game_state;
  static UI ui;
};
