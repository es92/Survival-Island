#ifndef WORLD_GEN_
#define WORLD_GEN_

#include "../chunk/blocks.h"
#include "../utils/utils.h"
#include "../utils/lazy_diamond_square.h"

// =============================

class World_Gen {
  public:
    World_Gen();
    Block get_block(XYZ xyz);
  private:
    Height_Map* height_map;
};


#endif
