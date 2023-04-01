#include <random>

class Random{
  public:
  static std::mt19937 engine;
  static int Random_Number(int range_start,int range_end)
  {
    std::uniform_int_distribution<int> dist{range_start,range_end};
    return dist(engine);
  }
};
