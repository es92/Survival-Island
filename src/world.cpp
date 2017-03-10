

#include "world.h"

#include <cstdlib>
using namespace std;

#define HILLS 0
#define CUBE_FIELD 1

#define MIDDLE_CUBE 2

bool get_block(World& world, int x, int y, int z){

  int mode = MIDDLE_CUBE;

  if (mode == HILLS) {
    x = (x + 64) % 128;
    z = (z + 64) % 128;

    x -= 64;
    z -= 64;

    x /= 4;
    z /= 4;

    return x*x + z*z < -y+32 || y == -32;
  } else if (mode == CUBE_FIELD) {
    return x % 4 == 0 && y % 4 == 0 && z % 4 == 0;
  } else if (mode == MIDDLE_CUBE) {
    return abs(x) < 16 && abs(y) < 16 && abs(z) < 16;
  }
}
