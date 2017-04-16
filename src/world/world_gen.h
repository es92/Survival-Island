#ifndef WORLD_GEN_
#define WORLD_GEN_

#include "../chunk/blocks.h"
#include "../utils/utils.h"
#include "../utils/lazy_diamond_square.h"

// =============================

class World_Gen {
  public:
    World_Gen(){
      Island_Height_Map* ihm = new Island_Height_Map;

      Height_Map* last = ihm;
      for (int i = 0; i < 4; i++){
        last = new Diamond_Square_Height_Map(*last);
      }

      height_map = last;
    };
    Block get_block(XYZ xyz);
  private:
    Height_Map* height_map;
};


#endif
