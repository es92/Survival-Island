
#include "automata.h"
#include "utils.h"

#include <boost/unordered_map.hpp>
#include <iostream>
using  boost::unordered::unordered_map;

using namespace std;

void step_automata(World& world, Automata& automata){

  unordered_map<XYZ, bool> changes;

  for (unordered_set<XYZ>::iterator it=world.db.maybe_needs_update.begin(); it != world.db.maybe_needs_update.end(); it++){
    XYZ xyz = *it;
    int x, y, z;
    tie(x, y, z) = xyz;
    bool x_pos_block = get_block(world, x+1, y, z);
    bool x_neg_block = get_block(world, x-1, y, z);
    bool y_pos_block = get_block(world, x, y+1, z);
    bool y_neg_block = get_block(world, x, y-1, z);
    bool z_pos_block = get_block(world, x, y, z+1);
    bool z_neg_block = get_block(world, x, y, z-1);
    if (x_pos_block || x_neg_block || y_pos_block || y_neg_block || z_pos_block || z_neg_block){
      changes.insert({ xyz, true });
    }
  }
  world.db.maybe_needs_update.clear();
  for (unordered_map<XYZ, bool>::iterator it=changes.begin(); it != changes.end(); it++){
    int x, y, z;
    tie(x, y, z) = it->first;
    set_block(world, it->second, x, y, z);

  }
}
