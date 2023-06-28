#include "Enemy.h"
#include "Player.h"
#include "Item.h"
#include "Json.h"
#include "Stats.h"
#include <Game.h>
#include <Random.h>
#include <string>
#include <vector>

Player Player::instance = Player("",Stats(),{0,0});
UI Game::ui;
Game::GameState Game::game_state;
std::vector<std::string> Game::ACTION_OPTIONS = {"ATTACK", "DODGE",     "BLOCK",
                                                 "MOVE",   "INVENTORY", "REST"};
std::vector<Item> Game::INVENTORY = Game::player.getInventory(); 
std::vector<std::string> Game::MAIN_MENU_OPTIONS = {
  "Load","New Game","Exit"
};
Enemy Game::current_Enemy;
Item Game::current_Item;
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
bool Game::isEnemyLocation(Location L)
{
  if(L.x == (Game::col_beg + (Game::current_Enemy.getLocation().x * 2)) && L.y == (Game::row_beg + (Game::current_Enemy.getLocation().y * 2)))
    return true;
  else
   return false;
}
bool Game::isPlayerLocation(Location L)
{
  if(L.x == (Game::col_beg + (Game::player.getLocation().x * 2)) && L.y == (Game::row_beg + (Game::player.getLocation().y * 2)))
    return true;
  else
   return false;
}
bool Game::isObstacleLocation(Location& L) {
    for (Location OL : Game::Obstacle_Locations) 
    {
      if (L.x == OL.x && L.y == OL.y)
      {
        return true;
      }
    }
    return false;
  }
int Game::col_beg,Game::row_beg;
std::vector<Location> Game::Obstacle_Locations;  
std::string Game::S = "YOU DIED!";
Player Game::player = Player::getInstance();
std::vector<std::string> Game::Enemy_list = {"Smough","Ornstien","Kaiden Warrior","Magic Archer","Noble Swordsman","CrestFallen Warrior"};
std::vector<Item> Game::Item_List = {
  {"Estus Flask",Item::Consumable ,{0,0,50,0,0,0,0,0},Slot::None},
  {"Rune Arc",Item::Consumable ,{20,20,20,20,20,20,20,20},Slot::None },
  {"Curved Sword",Item::Weapon,{0,0,0,0,85,0,0,0},Slot::Right_Hand}
};

