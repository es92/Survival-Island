

#include "world.h"
#include "../constants.h"

#include "../utils/timer.h"

#include <cstdlib>
#include <iostream>
using namespace std;


// =============================

void World_DB::maybe_gen_chunk(World_Gen& gen, XYZ xyz){
  int x, y, z;
  tie(x, y, z) = xyz;
  int cx = snap_to_chunk(x);
  int cy = snap_to_chunk(y);
  int cz = snap_to_chunk(z);
  XYZ chunk_xyz = XYZ(cx, cy, cz);


  if (initialized_chunks.find(chunk_xyz) == initialized_chunks.end()){
    lock.lock();
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
    lock.unlock();
  }

}

bool World_DB::has_block(World_Gen& gen, XYZ xyz){
  maybe_gen_chunk(gen, xyz);
  lock.lock_shared();
  bool r = blocks.find(xyz) != blocks.end();
  lock.unlock_shared();
  return r;
}

Block World_DB::get_block(World_Gen& gen, XYZ xyz){
  maybe_gen_chunk(gen, xyz);
  lock.lock_shared();

  if (blocks.find(xyz) == blocks.end()){
    lock.unlock_shared();
    return Empty_Block;
  } else {
    Block b = blocks.find(xyz)->second;
    lock.unlock_shared();
    return b;
  }
}


void World_DB::set_block(World_Gen& gen, Block b, XYZ xyz){
  maybe_gen_chunk(gen, xyz);
  lock.lock();
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
  lock.unlock();
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



  world.db.maybe_needs_update.insert(XYZ(x+0, y-1, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+1, y-1, z+0));
  world.db.maybe_needs_update.insert(XYZ(x-1, y-1, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+0, y-1, z+1));
  world.db.maybe_needs_update.insert(XYZ(x+0, y-1, z-1));

  world.db.maybe_needs_update.insert(XYZ(x+0, y+0, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+1, y+0, z+0));
  world.db.maybe_needs_update.insert(XYZ(x-1, y+0, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+0, y+0, z+1));
  world.db.maybe_needs_update.insert(XYZ(x+0, y+0, z-1));

  world.db.maybe_needs_update.insert(XYZ(x+0, y+1, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+1, y+1, z+0));
  world.db.maybe_needs_update.insert(XYZ(x-1, y+1, z+0));
  world.db.maybe_needs_update.insert(XYZ(x+0, y+1, z+1));
  world.db.maybe_needs_update.insert(XYZ(x+0, y+1, z-1));
}

unordered_set<XYZ> get_changed_chunks(World& world){
  return world.cc;
}

void reset_changed_chunks(World& world){
  world.cc.clear();
}
