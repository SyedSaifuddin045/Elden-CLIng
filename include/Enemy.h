#pragma once
#include "Entity.h"
#include "Location.h"
#include "Random.h"
#include <Stats.h>
#include <string>
#include <utility>

class Enemy :public Entitiy {
public:
  Enemy(std::string Enemy_Name, Stats &Enemy_Stats, Location &StartPos,int player_level)
      : Entitiy(Enemy_Name, Enemy_Stats, StartPos) {
    level = Random::Random_Number(1, player_level + 4);
    stat.generateRandomStats(player_level);
  }
  Enemy() : Entitiy() {
    level = 1;
  }
  std::string &getEnemyName() { return getName(); }
  Stats& getEnemyStats(){ return getStats();}
  Location& getEnemyLocation(){ return Entitiy::getLocation(); }
  int getLevel(){ return level; }
  void setEnemyLocation(Location L)
  {
    Entitiy::setLocation(L);
  }
  void setEnemyLocation(int x, int y)
  {
    Entitiy::setLocation(x,y);
  }
  //0 for did no move , 1 for Moved , >1 for damage
  std::pair<std::string,int> Turn(Entitiy& player)
  {
    int move = Random::Random_Number(0, 1);
    if(move == 1)
    {
    int action_index = Random::Random_Number(1,2);
    if(!checkInRange(player, 1) && action_index == 1)
    {
      Move(player.getLocation(), 1);
      return std::make_pair("Enemy moved from " + std::to_string(previous_location.x) + "," + std::to_string(previous_location.y) + " to " + std::to_string(location.x) + "," + std::to_string(location.y),1);
    }
    if(checkInRange(player, 1) && action_index == 2)
    {
      int damage = Attack(player, 1.0f);
      return std::make_pair("Enemy attacked player for "+std::to_string(damage)+"damage.",damage);
    }
    }
    return std::pair("Enemy did not act.",0);
  }
  bool checkInRange(Entitiy E,int range)
  {
    Location Entity_Location = E.getLocation();

    float distance = Entitiy::CalculateDistance(*this, E);
    return distance <= range;
  }
private:
  int level;
};
