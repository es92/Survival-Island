

#include "world.h"

#include <cstdlib>
using namespace std;

#define HILLS 0
#define CUBE_FIELD 1
#define SMALL_CUBE 2
#define FIELD 3

bool get_block(World& world, int x, int y, int z){

  int mode = FIELD;
  XYZ xyz(x, y, z);

  if (mode == HILLS) {
    x = (x + 64*1000000) % 128;
    z = (z + 64*1000000) % 128;

    x -= 64;
    z -= 64;

    x /= 4;
    z /= 4;

    return ((x*x + z*z < -y+32 || y == -32) && y > -33) ^ (world.toggled.find(xyz) != world.toggled.end());
  } else if (mode == CUBE_FIELD) {
    return (x % 4 == 0 && y % 4 == 0 && z % 4 == 0) ^ (world.toggled.find(xyz) != world.toggled.end());
  } else if (mode == SMALL_CUBE) {
    return abs(x) < 4 && abs(y) < 4 && abs(z) < 4;
  } else if (mode == FIELD) {
    return (y < -3) ^ (world.toggled.find(xyz) != world.toggled.end());
  }
}

void toggle_block(World& world, int x, int y, int z){
  XYZ xyz(x, y, z);
  if (world.toggled.find(xyz) == world.toggled.end()){
    world.toggled.insert(xyz);
  } else {
    world.toggled.erase(xyz);
  }
  world.cc.insert(XYZ(snap_to_chunk(x), snap_to_chunk(y), snap_to_chunk(z)));
}

unordered_set<XYZ> get_changed_chunks(World& world){
  return world.cc;
}

void reset_changed_chunks(World& world){
  world.cc.clear();
}
