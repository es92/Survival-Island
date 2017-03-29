#ifndef WORLD_
#define WORLD_

#include "utils.h"
#include <boost/unordered_set.hpp>
using boost::unordered_set;

#include <boost/unordered_map.hpp>
using  boost::unordered::unordered_map;

#include "blocks.h"

// =============================

class World_Gen {
  public:
    World_Gen(){ };
    Block get_block(XYZ xyz);
};

// =============================

class World_DB {
  public:
    World_DB(){ };
    bool has_block(World_Gen& gen, XYZ xyz);
    Block get_block(World_Gen& gen, XYZ xyz);
    void set_block(World_Gen& gen, Block b, XYZ xyz);
    unordered_set<XYZ> maybe_needs_update;
  private:
    void maybe_gen_chunk(World_Gen& gen, XYZ xyz);
    unordered_map<XYZ, Block> blocks;
    unordered_set<XYZ> initialized_chunks;
};

// =============================

typedef struct World_ {
  int x;
  World_DB db;
  World_Gen gen;
  unordered_set<XYZ> cc;
} World;

bool has_block(World& world, int x, int y, int z);
Block get_block(World& world, int x, int y, int z);
void set_block(World& world, Block b, int x, int y, int z);

// =============================

class Neighborhood {
  public:
    Neighborhood(World& _world, int _x, int _y, int _z) : world(_world), x(_x), y(_y), z(_z){ }
    XYZ xyz(){ return XYZ(x, y, z); }
    Block g(int dx, int dy, int dz){
      return get_block(world, x+dx, y+dy, z+dz);
    }
    bool empty(int dx, int dy, int dz){
      return get_block(world, x+dx, y+dy, z+dz).type == Block_Type::Empty;
    }
  private:
    World& world;
    int x;
    int y;
    int z;
};

// =============================

int get_block_tex_x(Block b);
int get_block_tex_y(Block b);


unordered_set<XYZ> get_changed_chunks(World& world);
void reset_changed_chunks(World& world);

#endif
