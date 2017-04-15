
#include "../chunk/blocks.h"
#include "../utils/utils.h"

#include <boost/unordered_map.hpp>
using  boost::unordered::unordered_map;

#include <boost/unordered_set.hpp>
using boost::unordered_set;

// =============================

class Height_Map {
  public:
    virtual int get_height(XYZ xyz){ return 0; }
};

class Island_Height_Map : public Height_Map {
  public:
    Island_Height_Map(){ };
    int get_height(XYZ xyz);
};

class Diamond_Square_Height_Map : public Height_Map {
  public:
    Diamond_Square_Height_Map(Island_Height_Map _parent) : parent(_parent){ };
    int get_height(XYZ xyz);
  private:
    unordered_set<XYZ> initialized_height_chunks;
    unordered_map<XYZ, int> height_map;
    Island_Height_Map parent;
};

class World_Gen {
  public:
    World_Gen(){ };
    Block get_block(XYZ xyz);
  private:
    Diamond_Square_Height_Map height_map = Diamond_Square_Height_Map(Island_Height_Map());
};

