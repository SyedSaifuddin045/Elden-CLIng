#pragma once
#include "Entity.h"
#include "Location.h"
#include "Random.h"
#include <Stats.h>
#include <string>

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
  void Turn(Entitiy player)
  {
    int move = Random::Random_Number(0, 1);
    if(move == 1)
    {
    int action_index = Random::Random_Number(1,2);
    if(!checkInRange(player, 1) && action_index == 1)
    {
      Move(player.getLocation(), 1); 
    }
    if(checkInRange(player, 1) && action_index == 2)
    {
      Attack(player, 1.0f);
    }
    }
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
