
#include "automata.h"

using namespace std;


Block_Condition EMPTY = [](const Block& b) -> bool { return b.type == Block_Type::Empty; };
Block_Condition NOT_EMPTY = [](const Block& b) -> bool { return b.type != Block_Type::Empty; };

Block_Condition EMPTY_OR_WATER = [](const Block& b) -> bool { return b.type == Block_Type::Empty 
                                                                  || b.type == Block_Type::Water; };
Block_Condition WATER = [](const Block& b) -> bool { return b.type == Block_Type::Water; };

int get_water_level(const Block& b){
  return EMPTY(b) ? 0 : water_block_level(b);
}

typedef tuple<int, int, int> Flows_x_z;
const int FLOW_DIRS = 4;
Flows_x_z flow_dirs[FLOW_DIRS] = {
  { 0, 1, 0 },
  { 3, -1, 0 },
  { 2, 0, 1 },
  { 5, 0, -1 }
};

int get_flow_in(const Neighborhood& N, int level){
  int flow_in = 0;
  if (WATER(N.g(0, 1, 0))){
    if (water_block_flows(N.g(0, 1, 0))[4] > 0){
      int water_level = get_water_level(N.g(0, 1, 0));
      flow_in += min((MAX_WATER_LEVEL - level), water_level);
    }
  }

  for (int i = 0; i < FLOW_DIRS; i++){
    int f;
    int x;
    int z;
    tie(f, x, z) = flow_dirs[i];
    
    if (WATER(N.g(-x, 0, -z))){
      Flows fl = water_block_flows(N.g(-x, 0, -z));
      if (fl[f] > 0){
        int water_level = get_water_level(N.g(-x, 0, -z));
        if (water_level > level){
          flow_in += floor((water_level + level)/2.) - level;
        }
      }
    }
  }

  return flow_in;
}


int get_flow_out(const Neighborhood& N, int level){
  int flow_out = 0;
  if (water_block_flows(N.g(0, 0, 0))[4] > 0 && EMPTY_OR_WATER(N.g(0, -1, 0))){
    int water_level = get_water_level(N.g(0, -1, 0));
    flow_out += min((MAX_WATER_LEVEL - water_level), level);
  }

  for (int i = 0; i < FLOW_DIRS; i++){
    int f;
    int x;
    int z;
    tie(f, x, z) = flow_dirs[i];
    Flows fl = water_block_flows(N.g(0, 0, 0));
    if (fl[f] > 0 && EMPTY_OR_WATER(N.g(x, 0, z))){
      int water_level = get_water_level(N.g(x, 0, z));
      if (water_level < level){
        flow_out += floor((water_level + level)/2.) - water_level;
      }
    }
  }

  return flow_out;
}

Flows get_flows(const Neighborhood& N, int level){
  if (EMPTY_OR_WATER(N.g(0, -1, 0))){
    int water_level = get_water_level(N.g(0, -1, 0));
    //int flow_out = min((MAX_WATER_LEVEL - water_level), level);
    return Flows({ 0, 0, 0, 0, 1, 0, });
  }

  for (int i = 0; i < FLOW_DIRS; i++){
    int f;
    int x;
    int z;
    tie(f, x, z) = flow_dirs[i];
    if (EMPTY_OR_WATER(N.g(x, 0, z))){
      int water_level = get_water_level(N.g(x, 0, z));
      if (water_level < level){
        Flows fl = Flows({ f==0, f==1, f==2, f==3, f==4, f==5, });
        return fl;
      }
    }
  }

  return Flows({0, 0, 0, 0, 0, 0});
}

#define BR(N) [](const Neighborhood& N) -> Block*

// ===========================================================

unordered_map<Block_Type, vector<Block_Rule> > rules({
  { Block_Type::Water, { 

    BR(N) {
      const Block& b = N.g(0, 0, 0);

      int level = water_block_level(b);

      int flow_in = get_flow_in(N, level);
      int flow_out = 0;

      flow_out += get_flow_out(N, level);

      Flows f = get_flows(N, level);// + flow_in - flow_out);

      if (level + flow_in - flow_out < 0){
        throw std::runtime_error( "invalid flow" );
      } else if (level + flow_in - flow_out == 0){
        return new Block(Empty_Block);
      } else {
        return new Block(mk_water_block(f, level + flow_in - flow_out));
      }
    }
  } },

  { Block_Type::Empty, {

    BR(N) {
      int flow_in = get_flow_in(N, 0);

      Flows f = get_flows(N, 0);

      if (flow_in != 0){
        return new Block(mk_water_block(f, flow_in));
      }
      return NULL;
    }
  } },
});
