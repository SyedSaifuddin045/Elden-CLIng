#include <Game.h>
#include <curses.h>
int main(int argc, char *argv[]) {
  Initialize();
  bool isCharacterMenu=false;
  Game::game_state = Game::GameState::Play_Game;

  while (Game::game_state != Game::GameState::End_Game) {
    switch (Game::game_state) {
    case Game::Game_Title:
      Game::ui.printAsciiArtInCenter(Game::Ascii_Title);
      curs_set(0);
      getch();
        wclear(stdscr);
          refresh();
        Game::game_state = Game::GameState::Main_Menu;
      break;
    case Game::Main_Menu:
      Game::ui.Clear();
      Game::ui.GenerateMainMenu(Game::MAIN_MENU_OPTIONS);
        Game::game_state = Game::GameState::Character_Select;
      break;
    case Game::Play_Game:
      Game::ui.Clear();
      Game::ui.GeneratePlayGameUI(Game::ACTION_OPTIONS, Game::INVENTORY);
        Game::game_state = Game::GameState::End_Screen;
      break;
    case Game::Character_Select:
      Game::ui.Clear();
      Game::ui.GenerateCharaterSelect(Game::CHARACTERS);
      getch();
      Game::game_state = Game::GameState::Play_Game;
      break;

    case Game::End_Screen:
      Game::ui.Clear();
      Game::ui.GenerateEndScreen();
      getch();
        Game::game_state = Game::GameState::End_Game;
      break;
      case Game::End_Game:
      Initialize::UnInitialize();
      return 0;
    }
  }
  Initialize::UnInitialize();
  return 0;
}
