
#ifndef WORLD_
#define WORLD_

typedef struct World_ {
  int x;
} World;

bool get_block(World& world, int x, int y, int z);

#endif
