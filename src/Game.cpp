#include <Game.h>
#include <Random.h>
#include <string>
#include <vector>

UI Game::ui;
Game::GameState Game::game_state;

std::vector<std::string> Game::ACTION_OPTIONS = {"ATTACK", "DODGE",     "BLOCK",
                                                 "MOVE",   "INVENTORY", "REST"};

std::vector<std::string> Game::INVENTORY = {
    "I 1", "I 2", "I 3", "I 4",  "I 5",  "I 6",
    "I 7", "I 8", "I 9", "I 10", "I 11",
};

std::vector<std::string> Game::MAIN_MENU_OPTIONS = {
  "New Game","Exit"
};

std::vector<std::string> Game::CHARACTERS = {
  "Character 1","Character 2","Character 3","Character 4","Character 5"
};

std::mt19937 Random::engine{std::random_device{}()};
int Game::game_row,Game::game_col;
