

#include "blocks.h"

Block Empty_Block = {
  .type = Block_Type::Empty,
};

Block Debug_Block = {
  .type = Block_Type::Debug,
};

Block Water_Block = {
  .type = Block_Type::Water,
};

Block mk_water_block(short water_level, bool on_ground){
  Block b;
  b.type = Block_Type::Water;
  b.data[0] = water_level;
  b.data[1] = on_ground;
  return b;
}

bool water_block_on_ground(Block b){
  return b.data[1] == 1;
}

short water_block_water_level(Block b){
  return b.data[0];
}
