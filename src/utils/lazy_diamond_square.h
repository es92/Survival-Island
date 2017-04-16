#ifndef LAZY_DIAMOND_SQUARE_
#define LAZY_DIAMOND_SQUARE_


#include "utils.h"

#include <boost/unordered_map.hpp>
using  boost::unordered::unordered_map;

#include <boost/unordered_set.hpp>
using boost::unordered_set;


class Height_Map {
  public:
    virtual float get_height(int x, int z){ return 0; }
};

class Island_Height_Map : public Height_Map {
  public:
    Island_Height_Map(){ };
    float get_height(int x, int z);
};

class Diamond_Square_Height_Map : public Height_Map {
  public:
    Diamond_Square_Height_Map(Height_Map& _parent, float _range) : parent(_parent), range(_range){ };
    float get_height(int x, int z);
    Height_Map& parent;
  private:
    float range;
    unordered_set<XZ> initialized_height_chunks;
    unordered_map<XZ, float> height_map;
};

void test_lazy_diamond_square();

#endif
