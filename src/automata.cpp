
#include "automata.h"
#include "utils.h"

#include <boost/unordered_map.hpp>
#include <iostream>
using  boost::unordered::unordered_map;

using namespace std;

void step_automata(World& world, Automata& automata){

  unordered_map<XYZ, Block> changes;

  for (unordered_set<XYZ>::iterator it=world.db.maybe_needs_update.begin(); it != world.db.maybe_needs_update.end(); it++){
    XYZ xyz = *it;
    int x, y, z;
    tie(x, y, z) = xyz;

    Block block = get_block(world, x, y, z);

    Block x_pos_block = get_block(world, x+1, y, z);
    Block x_neg_block = get_block(world, x-1, y, z);
    Block y_pos_block = get_block(world, x, y+1, z);
    Block y_neg_block = get_block(world, x, y-1, z);
    Block z_pos_block = get_block(world, x, y, z+1);
    Block z_neg_block = get_block(world, x, y, z-1);

    //if (block.type == Block_Type::Empty){
    //  bool on_ground = y_neg_block.type == Block_Type::Debug;
    //  if (y_pos_block.type == Block_Type::Water){
    //    changes.insert({ xyz, mk_water_block(10, on_ground) });
    //  } else if (water_block_on_ground(x_pos_block) && water_block_water_level(x_pos_block) > 0){
    //    changes.insert({ xyz, mk_water_block(water_block_water_level(x_pos_block)-1, on_ground) });
    //  } else if (water_block_on_ground(x_neg_block) && water_block_water_level(x_neg_block) > 0){
    //    changes.insert({ xyz, mk_water_block(water_block_water_level(x_neg_block)-1, on_ground) });
    //  }

    //} else if (block.type == Block_Type::Water){
    //  if (y_neg_block.type == Block_Type::Empty){
    //    changes.insert({ xyz, Empty_Block });
    //  } else if (water_block_on_ground(block)){
    //    if (x_pos_block.type == Block_Type::Empty && x_neg_block.type == Block_Type::Empty){
    //      changes.insert({ xyz, mk_water_block(water_block_water_level(block)-1, true) });
    //    } else if (x_neg_block.type == Block_Type::Empty){
    //      changes.insert({ xyz, mk_water_block(water_block_water_level(block)-1, true) });
    //    } else if (x_pos_block.type == Block_Type::Empty){
    //      changes.insert({ xyz, mk_water_block(water_block_water_level(block)-1, true) });
    //    }
    //  }
    //}
  }
  world.db.maybe_needs_update.clear();
  for (unordered_map<XYZ, Block>::iterator it=changes.begin(); it != changes.end(); it++){
    int x, y, z;
    tie(x, y, z) = it->first;
    Block b = it->second;
    set_block(world, b, x, y, z);
  }
}
