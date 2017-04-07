

#include "blocks.h"

using namespace std;

Block Empty_Block = {
  .type = Block_Type::Empty,
};

Block Debug_Block = {
  .type = Block_Type::Debug,
};

Block Water_Block = {
  .type = Block_Type::Water,
  { MAX_WATER_LEVEL, 0, 0, 0 },
};


Block Stone_Block = {
  .type = Block_Type::Stone
};

Block Sand_Block = {
  .type = Block_Type::Sand
};

Block Dirt_Block = {
  .type = Block_Type::Dirt
};

Block Grass_Block = {
  .type = Block_Type::Grass
};

bool operator==(const Block& a, const Block& b){
  if (a.type != b.type){
    return false;
  } else {
    for (int i = 0; i < 8; i++){
      if (a.data[i] != b.data[i]){
        return false;
      }
    }
  }
  return true;
}

bool operator!=(const Block& a, const Block& b){
  return !(a == b);
}

Block mk_water_block(Flows flows, int level){
  Block b;
  b.type = Block_Type::Water;
  b.data[0] = level;
  for (int i = 0; i < 6; i++){
    b.data[i+1] = flows[i];
  }
  return b;
}

Flows water_block_flows(const Block& b){
  Flows f;
  for (int i = 0; i < 6; i++){
    f[i] = b.data[i+1];
  }
  return f;
}

int water_block_level(const Block& b){
  return b.data[0];
}

// =============================

int get_block_tex_x(Block b){
  if (Block_Type::Debug == b.type){
    return 0;
  } else if (Block_Type::Water == b.type){
    return 1;
  } else if (Block_Type::Stone == b.type){
    return 2;
  } else if (Block_Type::Sand == b.type){
    return 3;
  } else if (Block_Type::Dirt == b.type){
    return 0;
  } else if (Block_Type::Grass == b.type){
    return 1;
  }
}

int get_block_tex_y(Block b){
  if (Block_Type::Debug == b.type){
    return 0;
  } else if (Block_Type::Water == b.type){
    return 0;
  } else if (Block_Type::Stone == b.type){
    return 0;
  } else if (Block_Type::Sand == b.type){
    return 0;
  } else if (Block_Type::Dirt == b.type){
    return 1;
  } else if (Block_Type::Grass == b.type){
    return 1;
  }
}

float get_block_height(Block b){
  if (Block_Type::Water == b.type){
    return min(water_block_level(b)*1. / MAX_WATER_LEVEL, 1.0);
  } else {
    return 1;
  }
}
