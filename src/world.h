
#ifndef WORLD_
#define WORLD_

#include "utils.h"
#include <boost/unordered_set.hpp>
using boost::unordered_set;

typedef struct World_ {
  int x;
  unordered_set<XYZ> toggled;
  unordered_set<XYZ> cc;
} World;

bool get_block(World& world, int x, int y, int z);
void toggle_block(World& world, int x, int y, int z);


unordered_set<XYZ> get_changed_chunks(World& world);
void reset_changed_chunks(World& world);

#endif
