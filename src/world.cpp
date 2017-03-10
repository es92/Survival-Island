

#include "world.h"

#include <cstdlib>
using namespace std;

#define HILLS 0
#define CUBE_FIELD 1

bool get_block(World& world, int x, int y, int z){

  int mode = CUBE_FIELD;

  if (mode == HILLS) {
    x = (x + 64) % 128;
    z = (z + 64) % 128;

    x -= 64;
    z -= 64;

    x /= 4;
    z /= 4;

    return x*x + z*z < -y+32 || y <= -32;
  } else if (mode == CUBE_FIELD) {
    return x % 4 == 0 && y % 4 == 0 && z % 4 == 0;
  }
}
