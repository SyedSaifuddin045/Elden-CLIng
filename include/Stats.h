#ifndef STATS_H
#define STATS_H
#include <Json.h>
#include <Random.h>
class Stats {
public:
  Stats()
    :max_HP(0),max_Stamina(0),current_HP(0),current_Stamina(0),Attack(0),Defense(0),HP_regain(0),Stamina_regain(0)
  {}
  void generateRandomStats(int level)
  {
    int seed = Random::Random_Number(1, level);

    int R_Max_HP = Random::Random_Number(seed*1,seed/2 * level);
    max_HP = R_Max_HP;
    int R_max_stamina = Random::Random_Number(seed*1,seed/2  * level) ;
    max_Stamina = R_max_stamina;
    int R_Attack = Random::Random_Number(seed*1,seed/2 * level) ;
    Attack = R_Attack;
    int R_Defense = Random::Random_Number(seed*1,seed/2 * level) ;
    Defense = R_Defense;
    int R_HP_regain = Random::Random_Number(seed*1,seed/2 * level) ;
    HP_regain = R_HP_regain;
    int R_Stamina_regain = Random::Random_Number(seed*1,seed/2 * level);
    Stamina_regain = R_Stamina_regain;
    current_HP = max_HP;
    current_Stamina = max_Stamina;
  }
  Stats operator+(const Stats& other) const {
        Stats result;

        result.max_HP = max_HP + other.max_HP;
        result.max_Stamina = max_Stamina + other.max_Stamina;
        result.current_HP = current_HP + other.current_HP;
        result.current_Stamina = current_Stamina + other.current_Stamina;
        result.Attack = Attack + other.Attack;
        result.Defense = Defense + other.Defense;
        result.HP_regain = HP_regain + other.HP_regain;
        result.Stamina_regain = Stamina_regain + other.Stamina_regain;

        return result;
    }
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
