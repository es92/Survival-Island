
#ifndef WORLD_
#define WORLD_

#include "utils.h"
#include <boost/unordered_set.hpp>
using boost::unordered_set;

class World_Gen {
  public:
    World_Gen(){ };
    bool get_block(XYZ xyz);
};

class World_DB {
  public:
    World_DB(){ };
    bool get_block(World_Gen& gen, XYZ xyz);
    void set_block(World_Gen& gen, bool exists, XYZ xyz);
    unordered_set<XYZ> maybe_needs_update;
  private:
    void maybe_gen_chunk(World_Gen& gen, XYZ xyz);
    unordered_set<XYZ> blocks;
    unordered_set<XYZ> initialized_chunks;
};

typedef struct World_ {
  int x;
  World_DB db;
  World_Gen gen;
  unordered_set<XYZ> cc;
} World;

bool get_block(World& world, int x, int y, int z);
void set_block(World& world, bool exists, int x, int y, int z);


unordered_set<XYZ> get_changed_chunks(World& world);
void reset_changed_chunks(World& world);

#endif
