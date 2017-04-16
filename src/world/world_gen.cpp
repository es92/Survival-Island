
#include "world_gen.h"
#include "math.h"
#include "../constants.h"

using namespace std;

Island_Height_Map ihm;

Block fixed_get_block(XYZ xyz);

// =============================


World_Gen::World_Gen(){
  Island_Height_Map* ihm = new Island_Height_Map;

  Height_Map* last = ihm;

  last = new Diamond_Square_Height_Map(*last, 4.0);
  last = new Diamond_Square_Height_Map(*last, 4.0);

  last = new Diamond_Square_Height_Map(*last, 2.0);
  last = new Diamond_Square_Height_Map(*last, 0.0);

  height_map = last;
};

Block World_Gen::get_block(XYZ xyz){
  //return fixed_get_block(xyz);

  int x, y, z;
  tie(x, y, z) = xyz;

  int height = height_map->get_height(x, z);

  if (height <= 0 && y <= 0){
    return Water_Block;
  } else if (height == 1 && y == 1){
    return Sand_Block;
  } else if (y == height){
    return Grass_Block;
  } else if (y == height - 1) {
    return Dirt_Block;
  } else if (y < height) {
    return Stone_Block;
  } else {
    return Empty_Block;
  }
}

// =============================

//
//void midpoint_square(int H, vector<vector<int>>& g){
//  //TODO needs square neighbors too
//  g[H/2][H/2] = (g[0][0] + g[0][H-1] + g[H-1][0] + g[H-1][H-1])/4;
//
//  g[0][H/2] = (g[0][0] + g[0][H-1] + g[H/2][H/2])/3;
//  g[H/2][0] = (g[0][0] + g[H-1][0] + g[H/2][H/2])/3;
//  g[H-1][H/2] = (g[H-1][0] + g[H-1][H-1] + g[H/2][H/2])/3;
//  g[H/2][H-1] = (g[0][H-1] + g[H-1][H-1] + g[H/2][H/2])/3;
//}
//
//int Diamond_Square_Height_Map::get_height(XYZ xyz){
//  int x, y, z;
//  tie(x, y, z) = xyz;
//
//  int cx = snap_to_chunk(x);
//  int cz = snap_to_chunk(z);
//
//  XYZ cxcz({cx, 0, cz});
//  XYZ xz({x, 0, z});
//
//  if (initialized_height_chunks.find(cxcz) == initialized_height_chunks.end()){
//
//    //int r = rand() % 16;
//    int r00 = parent.get_height({ cx/CHUNK_SIZE, 0, cz/CHUNK_SIZE });
//    int r01 = parent.get_height({ cx/CHUNK_SIZE, 0, cz/CHUNK_SIZE+1 });
//    int r10 = parent.get_height({ cx/CHUNK_SIZE+1, 0, cz/CHUNK_SIZE });
//    int r11 = parent.get_height({ cx/CHUNK_SIZE+1, 0, cz/CHUNK_SIZE+1 });
//
//    int H = 17;
//
//    vector<vector<int>> heights;
//
//    for (int xi = 0; xi < H; xi++){
//      heights.push_back({});
//      for (int zi = 0; zi < H; zi++){
//        heights[xi].push_back(0);
//      }
//    }
//
//    heights[0][0] = r00;
//    heights[0][16] = r01;
//    heights[16][0] = r10;
//    heights[16][16] = r11;
//
//    midpoint_square(H, heights);
//
//    for (int xi = cx; xi < cx + CHUNK_SIZE; xi++){
//      for (int zi = cz; zi < cz + CHUNK_SIZE; zi++){
//
//        int h = heights[xi-cx][zi-cz];
//
//        XYZ xizi({ xi, 0, zi });
//        height_map.insert({ xizi, h });
//      }
//    }
//
//    initialized_height_chunks.insert(cxcz);
//  }
//
//  int height = height_map.find(xz)->second;
//
//  return height;
//}

// =============================

#define HILLS 0
#define CUBE_FIELD 1
#define SMALL_CUBE 2
#define FIELD 3
#define SEED 4
#define ISLAND 5

Block fixed_get_block(XYZ xyz){
  int mode = ISLAND;
  int x, y, z;
  tie(x, y, z) = xyz;

  int H = 3;

  x /= 8;
  z /= 8;

  if (mode == ISLAND) {
    bool land = (sqrt(x*x + z*z) < -y+H) && y > -(H+1);
    if (land){
      if (y == -1) {
        return Sand_Block;
      } else if (y == 0){
        return Dirt_Block;
      } else if (y == 1){
        return Grass_Block;
      } else {
        return Stone_Block;
      }
    } else if (y < 0){
      return Water_Block;
    } else {
      return Empty_Block;
    }
  } else {
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
}

