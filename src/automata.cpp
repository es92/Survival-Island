
#include "automata.h"
#include "utils.h"

#include <boost/unordered_map.hpp>
#include <iostream>
using  boost::unordered::unordered_map;

using namespace std;

void step_automata(World& world, Automata& automata){

  unordered_map<XYZ, Block_Type> changes;

  for (unordered_set<XYZ>::iterator it=world.db.maybe_needs_update.begin(); it != world.db.maybe_needs_update.end(); it++){
    XYZ xyz = *it;
    int x, y, z;
    tie(x, y, z) = xyz;

    Block_Type block = get_block(world, x, y, z);

    Block_Type x_pos_block = get_block(world, x+1, y, z);
    Block_Type x_neg_block = get_block(world, x-1, y, z);
    Block_Type y_pos_block = get_block(world, x, y+1, z);
    Block_Type y_neg_block = get_block(world, x, y-1, z);
    Block_Type z_pos_block = get_block(world, x, y, z+1);
    Block_Type z_neg_block = get_block(world, x, y, z-1);

    if (block == Block_Type::Empty && y_pos_block == Block_Type::Water) {
      changes.insert({ xyz, Block_Type::Water });
    }
    if (block == Block_Type::Water && y_neg_block == Block_Type::Empty) {
      changes.insert({ xyz, Block_Type::Empty });
    }
  }
  world.db.maybe_needs_update.clear();
  for (unordered_map<XYZ, Block_Type>::iterator it=changes.begin(); it != changes.end(); it++){
    int x, y, z;
    tie(x, y, z) = it->first;
    Block_Type b = it->second;
    set_block(world, b, x, y, z);
  }
}
