#include "Item.h"
#include "Json.h"
#include <Game.h>
#include <Random.h>
#include <string>
#include <vector>

UI Game::ui;
Game::GameState Game::game_state;

std::vector<std::string> Game::ACTION_OPTIONS = {"ATTACK", "DODGE",     "BLOCK",
                                                 "MOVE",   "INVENTORY", "REST"};
std::vector<Item> Game::INVENTORY = Game::player.getInventory(); 
std::vector<std::string> Game::MAIN_MENU_OPTIONS = {
  "Load","New Game","Exit"
};

std::vector<std::string> Game::CHARACTERS = Json::GetAllJson(".");

std::mt19937 Random::engine{std::random_device{}()};
int Game::game_row,Game::game_col;
const char* Game::Ascii_Title = R"(
 _______  _        ______   _______  _       
(  ____ \( \      (  __  \ (  ____ \( (    /|
| (    \/| (      | (  \  )| (    \/|  \  ( |
| (__    | |      | |   ) || (__    |   \ | |
|  __)   | |      | |   | ||  __)   | (\ \) |
| (      | |      | |   ) || (      | | \   |
| (____/\| (____/\| (__/  )| (____/\| )  \  |
(_______/(_______/(______/ (_______/|/    )_)
                                             
 _______  _       _________ _        _______ 
(  ____ \( \      \__   __/( (    /|(  ____ \
| (    \/| (         ) (   |  \  ( || (    \/
| |      | |         | |   |   \ | || |      
| |      | |         | |   | (\ \) || | ____ 
| |      | |         | |   | | \   || | \_  )
| (____/\| (____/\___) (___| )  \  || (___) |
(_______/(_______/\_______/|/    )_)(_______)
                                             

)";
int Game::col_beg,Game::row_beg;
std::vector<Game::Location> Game::Obstacle_Locations;  
Player Game::player = Player::getInstance();
