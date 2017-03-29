

#include "blocks.h"

Block Empty_Block = {
  .type = Block_Type::Empty,
};

Block Debug_Block = {
  .type = Block_Type::Debug,
};

Block Water_Block = {
  .type = Block_Type::Water,
  { 0, 0, 0, 0 },
};

bool operator==(const Block& a, const Block& b){
  if (a.type != b.type){
    return false;
  } else {
    for (int i = 0; i < 4; i++){
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

//Block mk_water_block(Flow_Dir flow_dir){
//  Block b;
//  b.type = Block_Type::Water;
//  b.data[0] = flow_dir;
//  return b;
//}
//
//Flow_Dir water_block_flow_dir(Block b){
//  return static_cast<Flow_Dir>(b.data[0]);
//}

Block mk_water_block(Flow_Force flow_force){
  Block b;
  b.type = Block_Type::Water;
  short x, y, z;
  std::tie(x, y, z) = flow_force;
  b.data[0] = x;
  b.data[1] = y;
  b.data[2] = z;
  return b;
}

Flow_Force water_block_flow_force(Block b){
  return Flow_Force(b.data[0], b.data[1], b.data[2]);
}
