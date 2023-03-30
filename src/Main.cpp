#include <Game.h>

int main(int argc, char *argv[]) {
  Initialize();
  int c;
  Game::game_state = Game::GameState::Play_Game;
  while ((c=getch())!=10&&Game::game_state != Game::GameState::End_Screen) {
    Game::ui.GenerateUI(Game::ACTION_OPTIONS,Game::INVENTORY);
  }
  Initialize::UnInitialize();
  return 0;
}
