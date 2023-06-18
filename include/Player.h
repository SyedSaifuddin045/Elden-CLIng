#ifndef PLAYER_H
#define PLAYER_H
#include "Location.h"
#include <Item.h>
#include <string>
#include <Stats.h>
#include <vector>
#include <unordered_map>
#include <Json.h>
#include <Enemy.h>

class Player : public Entitiy {
public:
  static Player& getInstance() {
    return instance;
  }

  void setName(const std::string &name) { Name = name; }

  void setLevel(int lvl) { level = lvl; }

  void setStats(const Stats &stats) { stat = stats; }

  void addItemToInventory(const Item &item) { Inventory.push_back(item); }

  void equipItem(const Slot &slot, const Item &item) { Equipment[slot] = item; }

  std::string getName() const { return Name; }

  int getLevel() const { return level; }

  Stats getStats() const { return stat; }

  std::vector<Item> getInventory() const { return Inventory; }

  std::unordered_map<Slot, Item> getEquipment() const { return Equipment; }

  void setPlayerLocation(int x, int y)
  {
    Entitiy::setLocation(x,y);
  }
  void setPlayerLocation(Location& L)
  {
    Entitiy::setLocation(L);
  }
  Location& getPlayerLocation()
  {
    return Entitiy::getLocation();
  }

  Item getItemInSlot(const Slot &slot) const {
    if (Equipment.count(slot) > 0) {
      return Equipment.at(slot);
    } else {
      return Item();
    }
  }

  void ToJson(rapidjson::Value &value,
              rapidjson::Document::AllocatorType &allocator) const {
    value.SetObject();

    rapidjson::Value nameValue;
    nameValue.SetString(Name.c_str(), Name.size(), allocator);
    value.AddMember("Name", nameValue, allocator);

    value.AddMember("Level", level, allocator);

    rapidjson::Value statsValue(rapidjson::kObjectType);
    stat.ToJson(statsValue, allocator);
    value.AddMember("Stat", statsValue, allocator);

    rapidjson::Value invValue(rapidjson::kArrayType);
    for (const auto &item : Inventory) {
      rapidjson::Value itemValue(rapidjson::kObjectType);
      item.ToJson(itemValue, allocator);
      invValue.PushBack(itemValue, allocator);
    }
    value.AddMember("Inventory", invValue, allocator);

    rapidjson::Value equipValue(rapidjson::kObjectType);
    for (const auto &[slot, item] : Equipment) {
      rapidjson::Value slotValue;
      slotValue.SetString(SlotUtils::SlotToString(slot).c_str(),
                          SlotUtils::SlotToString(slot).size(), allocator);

      rapidjson::Value itemValue(rapidjson::kObjectType);
      item.ToJson(itemValue, allocator);

      equipValue.AddMember(slotValue, itemValue, allocator);
    }
    value.AddMember("Equipment", equipValue, allocator);
  }

  void FromJson(const rapidjson::Value &value) {
    setName(value["Name"].GetString());
    setLevel(value["Level"].GetInt());

    Stats stats;
    stats.FromJson(value["Stat"]);
    setStats(stats);

    Inventory.clear();
    const auto &invValue = value["Inventory"];
    for (rapidjson::SizeType i = 0; i < invValue.Size(); i++) {
      Item item;
      item.FromJson(invValue[i]);
      addItemToInventory(item);
    }

    Equipment.clear();
    const auto &equipValue = value["Equipment"];
    for (auto it = equipValue.MemberBegin(); it != equipValue.MemberEnd();
         it++) {
      Slot slot = SlotUtils::StringToSlot(it->name.GetString());
      Item item;
      item.FromJson(it->value);
      equipItem(slot, item);
    }
  }

private:
  Player(std::string _Name,Stats _stats,Location _location)
    :Entitiy(_Name,_stats,_location )
  {
    
  }
  int level;
  std::vector<Item> Inventory;
  std::unordered_map<Slot, Item> Equipment;
  static Player instance;
};
#endif // !PLAYER_H
