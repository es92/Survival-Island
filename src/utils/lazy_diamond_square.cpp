
#include "lazy_diamond_square.h"
#include "../world/world_gen.h"

float Island_Height_Map::get_height(int x, int z){
  return 12 - 8*sqrt(x*x + z*z);
}

int X = 0;

float Diamond_Square_Height_Map::get_height(int x, int z){

  int RES = 128;
  float r = (rand()%RES)*1.0 / RES * range;


  if (height_map.find(XZ({x, z})) == height_map.end()){
    if (x%2 == 0 && z%2 == 0){
      height_map.insert({ XZ({x, z}), parent.get_height(x/2, z/2) });
    } else if (x%2 != 0 && z%2 != 0){

      int x_target_0 = x - abs(x%2) + 0;
      int x_target_1 = x - abs(x%2) + 2;

      int z_target_0 = z - abs(z%2) + 0;
      int z_target_1 = z - abs(z%2) + 2;

      float h00 = get_height(x_target_0, z_target_0);
      float h01 = get_height(x_target_0, z_target_1);
      float h10 = get_height(x_target_1, z_target_0);
      float h11 = get_height(x_target_1, z_target_1);

      float h = (h00 + h01 + h10 + h11)/4.0 + r;

      height_map.insert({ XZ({x, z}), h });
    } else {

      int x_target_0 = x - 1;
      int x_target_1 = x + 1;

      int z_target_0 = z - 1;
      int z_target_1 = z + 1;

      float h00 = get_height(x_target_0, z);
      float h01 = get_height(x_target_1, z);
      float h10 = get_height(x, z_target_0);
      float h11 = get_height(x, z_target_1);

      float h = (h00 + h01 + h10 + h11)/4.0 + r;

      height_map.insert({ XZ({x, z}), h });
    }
  }
  float h = height_map.find(XZ({x, z}))->second;
  return h;
}

// =====================================

void test_lazy_diamond_square(){

  Island_Height_Map* ihm = new Island_Height_Map;
  Height_Map* dshm_1 = new Diamond_Square_Height_Map(*ihm, 0);
  Height_Map* dshm_2 = new Diamond_Square_Height_Map(*dshm_1, 0);
  Height_Map* dshm_3 = new Diamond_Square_Height_Map(*dshm_2, 0);
  Height_Map* dshm_4 = new Diamond_Square_Height_Map(*dshm_3, 0);
  Height_Map* dshm_5 = new Diamond_Square_Height_Map(*dshm_4, 0);
  Height_Map* dshm_6 = new Diamond_Square_Height_Map(*dshm_5, 0);

  Height_Map& h = *dshm_6;

  h.get_height(20, 20);

  exit(0);
}

