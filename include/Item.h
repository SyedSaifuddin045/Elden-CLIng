#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <Stats.h>

enum class Slot { None,Head, Torso, Hands, Legs, Right_Hand, Left_Hand };
class SlotUtils {
public:
  static Slot StringToSlot(const std::string &str) {
    if (str == "Head") {
      return Slot::Head;
    } else if (str == "Torso") {
      return Slot::Torso;
    } else if (str == "Hands") {
      return Slot::Hands;
    } else if (str == "Legs") {
      return Slot::Legs;
    } else if (str == "Right Hand") {
      return Slot::Right_Hand;
    } else if (str == "Left Hand") {
      return Slot::Left_Hand;
    } else if (str == "None") {
      return Slot::None;
    } else {
      throw std::invalid_argument("Invalid slot name: " + str);
    }
  }
  static std::string SlotToString(Slot slot) {
    switch (slot) {
    case Slot::Head:
      return "Head";
    case Slot::Torso:
      return "Torso";
    case Slot::Hands:
      return "Hands";
    case Slot::Legs:
      return "Legs";
    case Slot::Right_Hand:
      return "Right Hand";
    case Slot::Left_Hand:
      return "Left Hand";
    case Slot::None:
      return "None";
    default:
      return "Unknown";
    }
  }
};
class Item {
public:
  std::string Name;
  enum Item_Class { Armour, Weapon, Consumable } Class;
  Stats Stat_Change;
  Slot Item_slot;
  void ToJson(rapidjson::Value &value,
              rapidjson::Document::AllocatorType &allocator) const {
    value.SetObject();
    value.AddMember("Name", rapidjson::Value(Name.c_str(), allocator).Move(),
                    allocator);
    value.AddMember("Class", static_cast<int>(Class), allocator);
    rapidjson::Value stat_change(rapidjson::kObjectType);
    Stat_Change.ToJson(stat_change, allocator);
    value.AddMember("Stat_Change", stat_change, allocator);
    value.AddMember("Item_slot", static_cast<int>(Item_slot), allocator);
  }

  void FromJson(const rapidjson::Value &value) {
    Name = value["Name"].GetString();
    Class = static_cast<Item_Class>(value["Class"].GetInt());
    Stat_Change.FromJson(value["Stat_Change"]);
    Item_slot = static_cast<Slot>(value["Item_slot"].GetInt());
  }
};

#endif // !ITEM_H
