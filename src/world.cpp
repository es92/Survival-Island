

#include "world.h"

#include <cstdlib>
using namespace std;

bool get_block(World& world, int x, int y, int z){

  x = (x + 64) % 128;
  z = (z + 64) % 128;

  x -= 64;
  z -= 64;

  x /= 4;
  z /= 4;

  return x*x + z*z < -y+32 || y <= -32;
}
