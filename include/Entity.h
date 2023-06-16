#include "Location.h"
#include "Stats.h"
#include <string>
class Entitiy{
  protected:
    std::string Name;
    Stats stat;
    Location location;
  public:
    Entitiy(std::string& _Name , Stats& Stat , Location& _location)
      :Name(_Name),stat(Stat),location(_location)
    {
    }
    Entitiy()
    {
      Name = "";
      stat = Stats();
      location = Location({0,0});
    }
    void Attack(Entitiy& entity,float multiplier)
    {
      int damage = multiplier * stat.Attack;
      entity.TakeDamage(damage);
    }
    void TakeDamage(int damage)
    {
      if(stat.current_HP - damage <= 0)
      {
        stat.current_HP = 0;
      }
      else {
      stat.current_HP -= damage; 
      }
    }
    std::string& getName()
    {
      return Name;
    }
    Stats& getStats()
    {
      return stat;
    }
    Location& getLocation()
    {
      return location;
    }
    void setLocation(int x,int y)
    {
      location.x = x;
      location.y = y;
    }
    void setLocation(Location L)
    {
      location = L;
    }
};
