

#include "world.h"

#include <cstdlib>
using namespace std;

#define HILLS 0
#define CUBE_FIELD 1
#define SMALL_CUBE 2

bool get_block(World& world, int x, int y, int z){

  int mode = HILLS;

  if (mode == HILLS) {
    x = (x + 64*1000000) % 128;
    z = (z + 64*1000000) % 128;

    x -= 64;
    z -= 64;

    x /= 4;
    z /= 4;

    return (x*x + z*z < -y+32 || y == -32) && y > -33;
  } else if (mode == CUBE_FIELD) {
    return x % 4 == 0 && y % 4 == 0 && z % 4 == 0;
  } else if (mode == SMALL_CUBE) {
    return abs(x) < 4 && abs(y) < 4 && abs(z) < 4;
  }
}
