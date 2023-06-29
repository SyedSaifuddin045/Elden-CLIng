#pragma once
#include "Game.h"
#include "Location.h"
#include "Stats.h"
#include <cmath>
#include <string>

class Entitiy {
protected:
  std::string Name;
  Stats stat;
  Location location, previous_location;

public:
  Entitiy(std::string &_Name, Stats &Stat, Location &_location)
      : Name(_Name), stat(Stat), location(_location) {}
  Entitiy() {
    Name = "";
    stat = Stats();
    location = Location({0, 0});
  }
  int Attack(Entitiy &entity, float multiplier) {
    int damage = multiplier * stat.Attack;
    if (!((entity.stat.Defense / 2) > damage))
      damage = damage - (entity.stat.Defense / 2);
    else {
      entity.TakeDamage(1);
      return 1;
    }
    entity.TakeDamage(damage);
    return damage;
  }
  void RecoverHP(int HPamount) {
    stat.current_HP += HPamount;
    if (stat.current_HP > stat.max_HP) {
      stat.current_HP = stat.max_HP;
    }
  }
  void TakeDamage(int damage) {
    if (stat.current_HP - damage <= 0) {
      stat.current_HP = 0;
    } else {
      stat.current_HP -= damage;
    }
  }
  std::string &getName() { return Name; }
  Stats &getStats() { return stat; }
  Location &getLocation() { return location; }
  void setLocation(int x, int y) {
    location.x = x;
    location.y = y;
  }
  void setLocation(Location L) { location = L; }
  static float CalculateDistance(Entitiy E1, Entitiy E2) {
    int dx = E1.getLocation().x - E2.getLocation().x;
    int dy = E1.getLocation().y - E2.getLocation().y;
    return std::sqrt(dx * dx + dy * dy);
  }
  void Move(Location &L, int increment) {
    int dy = L.y - location.y;
    int dx = L.x - location.x;
    previous_location = location;

    // Move towards the target location if no obstacle is present
    if (dx != 0)
      location.x += dx > 0 ? 1 : -1;
    if (dy != 0)
      location.y += dy > 0 ? 1 : -1;

    // Check if the current location is an obstacle
    Location currentLocation = location;
    if (Game::isGameObstacle(currentLocation) ||
        Game::isPlayerLocation(currentLocation)) {
      // Find the closest non-obstacle location to the target location
      Location closestLocation;
      int minDistance = INT_MAX;

      for (int i = -increment; i <= increment; i++) {
        for (int j = -increment; j <= increment; j++) {
          Location newLocation = {L.x + i, L.y + j};
          if (newLocation.x >= 0 && newLocation.y >= 0 &&
              !Game::isGameObstacle(newLocation) &&
              !Game::isPlayerLocation(newLocation)) {
            int distance = abs(newLocation.x - location.x) +
                           abs(newLocation.y - location.y);
            if (distance < minDistance) {
              closestLocation = newLocation;
              minDistance = distance;
            }
          }
        }
      }

      // Update the current location to the closest non-obstacle location
      location = closestLocation;
    }
  }

  void setPreviousLocation(Location L) { previous_location = L; }
  Location getPreviousLocation() { return previous_location; }
};
