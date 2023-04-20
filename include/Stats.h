#ifndef STATS_H
#define STATS_H
#include <Json.h>
class Stats {
public:
  Stats()
    :max_HP(0),max_Stamina(0),current_HP(0),current_Stamina(0),Attack(0),Defense(0),HP_regain(0),Stamina_regain(0)
  {}
  int max_HP;
  int max_Stamina;
  int current_HP;
  int current_Stamina;
  int Attack;
  int Defense;
  int HP_regain;
  int Stamina_regain;

  void ToJson(rapidjson::Value &value,
              rapidjson::Document::AllocatorType &allocator) const {
    value.SetObject();
    value.AddMember("max_HP", max_HP, allocator);
    value.AddMember("max_Stamina", max_Stamina, allocator);
    value.AddMember("current_HP", current_HP, allocator);
    value.AddMember("current_Stamina", current_Stamina, allocator);
    value.AddMember("Attack", Attack, allocator);
    value.AddMember("Defense", Defense, allocator);
    value.AddMember("HP_regain", HP_regain, allocator);
    value.AddMember("Stamina_regain", Stamina_regain, allocator);
  }

  void FromJson(const rapidjson::Value &value) {
    max_HP = value["max_HP"].GetInt();
    max_Stamina = value["max_Stamina"].GetInt();
    current_HP = value["current_HP"].GetInt();
    current_Stamina = value["current_Stamina"].GetInt();
    Attack = value["Attack"].GetInt();
    Defense = value["Defense"].GetInt();
    HP_regain = value["HP_regain"].GetInt();
    Stamina_regain = value["Stamina_regain"].GetInt();
  }
};
#endif // !STATS_H
