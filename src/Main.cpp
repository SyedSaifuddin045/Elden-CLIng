#include <Game.h>

int main(int argc, char *argv[]) {
  Initialize();
  int c;
  Game::game_state = Game::GameState::Game_Title;

  while (Game::game_state != Game::GameState::End_Game) {
    switch (Game::game_state) {
    case Game::Game_Title:
      Game::ui.printAsciiArtInCenter(Game::Ascii_Title);
      c = getch();
      if (c == 10)
      {
        wclear(stdscr);
          refresh();
        Game::game_state = Game::GameState::Main_Menu;
      }
      break;
    case Game::Main_Menu:
      Game::ui.Clear();
      Game::ui.GenerateMainMenu(Game::MAIN_MENU_OPTIONS);
      c = getch();
      if (c == 10)
        Game::game_state = Game::GameState::Character_Select;
      break;
    case Game::Play_Game:
      Game::ui.Clear();
      Game::ui.GeneratePlayGameUI(Game::ACTION_OPTIONS, Game::INVENTORY);
      c = getch();
      if (c == 10)
        Game::game_state = Game::GameState::End_Screen;
      break;
    case Game::Character_Select:
      Game::ui.Clear();
      Game::ui.GenerateCharaterSelect(Game::CHARACTERS);
      c = getch();
      if (c == 10)
        Game::game_state = Game::GameState::Play_Game;
      break;

    case Game::End_Screen:
      Game::ui.Clear();
      Game::ui.GenerateEndScreen();
      c = getch();
      if (c == 10)
        Game::game_state = Game::GameState::End_Game;
      break;
    }
  }
  Initialize::UnInitialize();
  return 0;
}
