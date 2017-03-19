

#include "world.h"
#include "constants.h"

#include <cstdlib>
#include <iostream>
using namespace std;

#define HILLS 0
#define CUBE_FIELD 1
#define SMALL_CUBE 2
#define FIELD 3
#define SEED 4

// =============================

int get_block_tex_x(Block b){
  if (Block_Type::Debug == b.type){
    return 0;
  } else if (Block_Type::Water == b.type){
    return 1;
  }
}

int get_block_tex_y(Block b){
  if (Block_Type::Debug == b.type){
    return 0;
  } else if (Block_Type::Water == b.type){
    return 0;
  }
}

// =============================

void World_DB::maybe_gen_chunk(World_Gen& gen, XYZ xyz){
  int x, y, z;
  tie(x, y, z) = xyz;
  int cx = snap_to_chunk(x);
  int cy = snap_to_chunk(y);
  int cz = snap_to_chunk(z);
  XYZ chunk_xyz = XYZ(cx, cy, cz);

  if (initialized_chunks.find(chunk_xyz) == initialized_chunks.end()){
    for (int x = 0; x < CHUNK_SIZE; x++){
      for (int y = 0; y < CHUNK_SIZE; y++){
        for (int z = 0; z < CHUNK_SIZE; z++){
          XYZ block_xyz = XYZ(cx+x, cy+y, cz+z);
          Block b = gen.get_block(block_xyz);
          if (b.type != Block_Type::Empty){
            blocks.insert({ block_xyz, b });

            //maybe_needs_update.insert(XYZ(cx+x+1, cy+y+0, cz+z+0));
            //maybe_needs_update.insert(XYZ(cx+x-1, cy+y+0, cz+z+0));
            //maybe_needs_update.insert(XYZ(cx+x+0, cy+y+1, cz+z+0));
            //maybe_needs_update.insert(XYZ(cx+x+0, cy+y-1, cz+z+0));
            //maybe_needs_update.insert(XYZ(cx+x+0, cy+y+0, cz+z+1));
            //maybe_needs_update.insert(XYZ(cx+x+0, cy+y+0, cz+z-1));

          }
        }
      }
    }
    initialized_chunks.insert(chunk_xyz);
  }

}

bool World_DB::has_block(World_Gen& gen, XYZ xyz){
  maybe_gen_chunk(gen, xyz);
  return blocks.find(xyz) != blocks.end();
}

Block World_DB::get_block(World_Gen& gen, XYZ xyz){
  maybe_gen_chunk(gen, xyz);
  if (blocks.find(xyz) == blocks.end()){
    return Empty_Block;
  } else {
    return blocks.find(xyz)->second;
  }
}


void World_DB::set_block(World_Gen& gen, Block b, XYZ xyz){
  maybe_gen_chunk(gen, xyz);
  if (b.type != Block_Type::Empty){
    if (blocks.find(xyz) != blocks.end()){
      blocks.erase(xyz);
    }
    blocks.insert({xyz, b});
  } else {
    if (blocks.find(xyz) != blocks.end()){
      blocks.erase(xyz);
    }
  }
}

// =============================

Block World_Gen::get_block(XYZ xyz){
  int mode = FIELD;
  int x, y, z;
  tie(x, y, z) = xyz;

  bool exists;

  if (mode == HILLS) {
    x = (x + 64*1000000) % 128;
    z = (z + 64*1000000) % 128;

    x -= 64;
    z -= 64;

    x /= 4;
    z /= 4;

    exists = (x*x + z*z < -y+32 || y == -32) && y > -33;
  } else if (mode == CUBE_FIELD) {
    exists = (x % 4 == 0 && y % 4 == 0 && z % 4 == 0);
  } else if (mode == SMALL_CUBE) {
    exists = abs(x) < 4 && abs(y) < 4 && abs(z) < 4;
  } else if (mode == FIELD) {
    exists = (y < -3);
  } else if (mode == SEED) {
    exists = x == 1 && y == -1 && z == -6;
  }
  if (exists){
    return Debug_Block;
  } else {
    return Empty_Block;
  }
}

// =============================

bool has_block(World& world, int x, int y, int z){
  return world.db.has_block(world.gen, XYZ(x, y, z));
}

Block get_block(World& world, int x, int y, int z){
  return world.db.get_block(world.gen, XYZ(x, y, z));
}

void set_block(World& world, Block b, int x, int y, int z){
  world.db.set_block(world.gen, b, XYZ(x, y, z));

  world.cc.insert(XYZ(snap_to_chunk(x), snap_to_chunk(y), snap_to_chunk(z)));
  if (x == 0){
    world.cc.insert(XYZ(snap_to_chunk(x-1), snap_to_chunk(y), snap_to_chunk(z)));
  } else if (x == CHUNK_SIZE-1){
    world.cc.insert(XYZ(snap_to_chunk(x+1), snap_to_chunk(y), snap_to_chunk(z)));
  }
  if (y == 0){
    world.cc.insert(XYZ(snap_to_chunk(x), snap_to_chunk(y-1), snap_to_chunk(z)));
  } else if (y == CHUNK_SIZE-1){
    world.cc.insert(XYZ(snap_to_chunk(x), snap_to_chunk(y+1), snap_to_chunk(z)));
  }
  if (z == 0){
    world.cc.insert(XYZ(snap_to_chunk(x), snap_to_chunk(y), snap_to_chunk(z-1)));
  } else if (z == CHUNK_SIZE-1){
    world.cc.insert(XYZ(snap_to_chunk(x), snap_to_chunk(y), snap_to_chunk(z+1)));
  }


  world.db.maybe_needs_update.insert(XYZ(x+0, y+0, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+1, y+0, z+0));
  world.db.maybe_needs_update.insert(XYZ(x-1, y+0, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+0, y+1, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+0, y-1, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+0, y+0, z+1));
  world.db.maybe_needs_update.insert(XYZ(x+0, y+0, z-1));
}

unordered_set<XYZ> get_changed_chunks(World& world){
  return world.cc;
}

void reset_changed_chunks(World& world){
  world.cc.clear();
}
