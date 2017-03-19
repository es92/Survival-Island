
#ifndef WORLD_
#define WORLD_

#include "utils.h"
#include <boost/unordered_set.hpp>
using boost::unordered_set;

#include <boost/unordered_map.hpp>
using  boost::unordered::unordered_map;

enum Block_Type { Empty, Debug, Water };

// =============================

class World_Gen {
  public:
    World_Gen(){ };
    Block_Type get_block(XYZ xyz);
};

// =============================

class World_DB {
  public:
    World_DB(){ };
    bool has_block(World_Gen& gen, XYZ xyz);
    Block_Type get_block(World_Gen& gen, XYZ xyz);
    void set_block(World_Gen& gen, Block_Type b, XYZ xyz);
    unordered_set<XYZ> maybe_needs_update;
  private:
    void maybe_gen_chunk(World_Gen& gen, XYZ xyz);
    unordered_map<XYZ, Block_Type> blocks;
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
Block_Type get_block(World& world, int x, int y, int z);
void set_block(World& world, Block_Type b, int x, int y, int z);

// =============================

int get_block_tex_x(Block_Type b);
int get_block_tex_y(Block_Type b);


unordered_set<XYZ> get_changed_chunks(World& world);
void reset_changed_chunks(World& world);

#endif
