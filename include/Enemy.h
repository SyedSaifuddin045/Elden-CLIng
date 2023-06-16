#pragma once
#include "Entity.h"
#include "Location.h"
#include "Random.h"
#include <Stats.h>
#include <string>

class Enemy : Entitiy {
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
private:
  int level;
};
