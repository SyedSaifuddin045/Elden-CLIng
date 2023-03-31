#include <Game.h>

int main(int argc, char *argv[]) {
  Initialize();
  int c;
  Game::game_state = Game::GameState::Main_Menu;

  while ((c = getch()) != KEY_F(1) &&
         Game::game_state != Game::GameState::End_Screen) {
    switch (Game::game_state) {
    case Game::Main_Menu:
      Game::ui.GenerateMainMenu(Game::MAIN_MENU_OPTIONS);
      if(c == 10)
        Game::game_state = Game::GameState::Character_Select;
      break;
    case Game::Play_Game:
      Game::ui.Clear();
      Game::ui.GenerateUI(Game::ACTION_OPTIONS, Game::INVENTORY);
      if(c == 10)
        Game::game_state = Game::GameState::End_Screen;
      break;
    case Game::Character_Select:
      Game::ui.Clear();
      Game::ui.GenerateCharaterSelect(Game::CHARACTERS);
      if(c == 10)
        Game::game_state = Game::GameState::Play_Game;
      break;

    case Game::End_Screen:
      Game::ui.Clear();

      break;
    }
  }
  Initialize::UnInitialize();
  return 0;
}
