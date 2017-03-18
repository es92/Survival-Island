

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
          bool exists = gen.get_block(block_xyz);
          if (exists){
            blocks.insert(block_xyz);

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

bool World_DB::get_block(World_Gen& gen, XYZ xyz){
  maybe_gen_chunk(gen, xyz);
  return blocks.find(xyz) != blocks.end();
}

void World_DB::set_block(World_Gen& gen, bool exists, XYZ xyz){
  maybe_gen_chunk(gen, xyz);
  if (exists){
    blocks.insert(xyz);
  } else {
    if (blocks.find(xyz) != blocks.end()){
      blocks.erase(xyz);
    }
  }
}

// =============================

bool World_Gen::get_block(XYZ xyz){
  int mode = SEED;
  int x, y, z;
  tie(x, y, z) = xyz;

  if (mode == HILLS) {
    x = (x + 64*1000000) % 128;
    z = (z + 64*1000000) % 128;

    x -= 64;
    z -= 64;

    x /= 4;
    z /= 4;

    return (x*x + z*z < -y+32 || y == -32) && y > -33;
  } else if (mode == CUBE_FIELD) {
    return (x % 4 == 0 && y % 4 == 0 && z % 4 == 0);
  } else if (mode == SMALL_CUBE) {
    return abs(x) < 4 && abs(y) < 4 && abs(z) < 4;
  } else if (mode == FIELD) {
    return (y < -3);
  } else if (mode == SEED) {
    return x == 1 && y == -1 && z == -6;
  }
}

// =============================

bool get_block(World& world, int x, int y, int z){
  return world.db.get_block(world.gen, XYZ(x, y, z));
}

void set_block(World& world, bool exists, int x, int y, int z){
  world.db.set_block(world.gen, exists, XYZ(x, y, z));

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
