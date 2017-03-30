
#include "automata.h"
#include "utils.h"

#include <boost/unordered_map.hpp>
#include <iostream>
using  boost::unordered::unordered_map;

using namespace std;

//bool Simple_And_Condition::matches(Neighborhood& N){
//  for (int i = 0; i < block_conditions.size(); i++){
//    int x, y, z;
//    tie(x, y, z) = block_conditions[i].first;
//    if (!block_conditions[i].second(N.g(x, y, z))){
//      return false;
//    }
//  }
//  return true;
//}


void step_automata(World& world, Automata& automata){

  unordered_map<XYZ, Block> changes;

  for (unordered_set<XYZ>::iterator it=world.db.maybe_needs_update.begin(); it != world.db.maybe_needs_update.end(); it++){
    XYZ xyz = *it;
    int x, y, z;
    tie(x, y, z) = xyz;

    Neighborhood N(world, x, y, z);

    if (rules.find(N.g(0, 0, 0).type) != rules.end()){
      vector<Block_Rule> block_rules = rules.find(N.g(0, 0, 0).type)->second;

      for (int i = 0; i < block_rules.size(); i++){
        Block_Rule& br = block_rules[i];

        Block* b = br(N);

        if (b != NULL){
          Block bb = *b;
          delete b;
          changes.insert({ xyz, bb });
        }

        //if (c->type() == Conditions::Simple_And) {
        //  Simple_And_Condition* mc = static_cast<Simple_And_Condition*>(c);

        //  if (mc->matches(N)){
        //    changes.insert({ xyz, block_rules[i].result(N) });
        //    break;
        //  }
        //} else if (c->type() == Conditions::Custom) {
        //  Custom_Condition* mc = static_cast<Custom_Condition*>(c);

        //  if (mc->matches(N)){
        //    changes.insert({ xyz, block_rules[i].result(N) });
        //    break;
        //  }

        //} else {
        //  throw runtime_error("could not find condition");
        //}
      }
    }
  }
  world.db.maybe_needs_update.clear();
  for (unordered_map<XYZ, Block>::iterator it=changes.begin(); it != changes.end(); it++){
    int x, y, z;
    tie(x, y, z) = it->first;
    Block old = get_block(world, x, y, z);
    Block b = it->second;
    if (old != b){
      set_block(world, b, x, y, z);
    }
  }
}
