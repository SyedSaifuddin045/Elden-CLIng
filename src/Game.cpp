#include <Game.h>

UI Game::ui;
Game::GameState Game::game_state;

std::vector<std::string> Game::ACTION_OPTIONS = {"ATTACK", "DODGE",     "BLOCK",
                                                 "MOVE",   "INVENTORY", "REST"};

std::vector<std::string> Game::INVENTORY = {
    "I 1", "I 2", "I 3", "I 4",  "I 5",  "I 6",
    "I 7", "I 8", "I 9", "I 10", "I 11",
};
