
#include "automata.h"
#include "utils.h"

#include <boost/unordered_map.hpp>
#include <iostream>
using  boost::unordered::unordered_map;

using namespace std;

//void add_water_block(unordered_map<XYZ, Block>& changes, Neighborhood& N){
//  if (N.empty(0, -1, 0)){
//    changes.insert({ N.xyz(), mk_water_block(Flow_Dir::Y_Neg) });
//  } else if (N.empty(1, 0, 0) && N.empty(1, -1, 0)){
//    changes.insert({ N.xyz(), mk_water_block(Flow_Dir::X_Pos) });
//  } else if (N.empty(-1, 0, 0) && N.empty(-1, -1, 0)){
//    changes.insert({ N.xyz(), mk_water_block(Flow_Dir::X_Neg) });
//  } else if (N.empty(0, 0, 1) && N.empty(0, -1, 1)){
//    changes.insert({ N.xyz(), mk_water_block(Flow_Dir::Z_Pos) });
//  } else if (N.empty(0, 0, -1) && N.empty(0, -1, -1)){
//    changes.insert({ N.xyz(), mk_water_block(Flow_Dir::Z_Neg) });
//  } else {
//    changes.insert({ N.xyz(), mk_water_block(Flow_Dir::None) });
//  }
//}

bool Simple_And_Condition::matches(Neighborhood& N){
  for (int i = 0; i < block_conditions.size(); i++){
    int x, y, z;
    tie(x, y, z) = block_conditions[i].first;
    if (!block_conditions[i].second(N.g(x, y, z))){
      return false;
    }
  }
  return true;
}


void step_automata(World& world, Automata& automata){

  unordered_map<XYZ, Block> changes;

  //typedef tuple<Flow_Dir, int, int> Flow_Dir_x_z;
  //Flow_Dir_x_z flow_dirs[5] = {
  //  { Flow_Dir::Y_Neg, 0, 0 },
  //  { Flow_Dir::X_Pos, 1, 0 },
  //  { Flow_Dir::X_Neg, -1, 0 },
  //  { Flow_Dir::Z_Pos, 0, 1 },
  //  { Flow_Dir::Z_Neg, 0, -1 }
  //};

  for (unordered_set<XYZ>::iterator it=world.db.maybe_needs_update.begin(); it != world.db.maybe_needs_update.end(); it++){
    XYZ xyz = *it;
    int x, y, z;
    tie(x, y, z) = xyz;

    Neighborhood N(world, x, y, z);


    if (rules.find(N.g(0, 0, 0).type) != rules.end()){
      vector<Rule> block_rules = rules.find(N.g(0, 0, 0).type)->second;

      for (int i = 0; i < block_rules.size(); i++){
        Condition* c = block_rules[i].condition;

        if (c->type() == Conditions::Simple_And) {
          Simple_And_Condition* mc = static_cast<Simple_And_Condition*>(c);

          if (mc->matches(N)){
            changes.insert({ xyz, block_rules[i].result });
            break;
          }
        } else {
          throw runtime_error("could not find condition");
        }
      }
    }

    //if (N.g(0, 0, 0).type == Block_Type::Water){
    //  Flow_Force current_ff = water_block_flow_force(N.g(0, 0, 0));

    //  if (get<1>(current_ff) < 0 && N.g(0, -1, 0).type == Block_Type::Empty){
    //    changes.insert({ xyz, Empty_Block });
    //  } else if (get<0>(current_ff) > 0 && N.g(1, 0, 0).type == Block_Type::Empty){
    //    changes.insert({ xyz, Empty_Block });
    //  } else {
    //    Flow_Force computed_ff(0, 0, 0);

    //    if (N.g(0, -1, 0).type == Block_Type::Empty){
    //      get<1>(computed_ff) -= 1;
    //    }

    //    if (N.g(0, 1, 0).type == Block_Type::Water){
    //      Flow_Force n_ff = water_block_flow_force(N.g(0, 1, 0));
    //      if (get<1>(n_ff) <= 0){
    //        get<1>(computed_ff) += get<1>(n_ff) - 1;
    //      }
    //    }

    //    if (N.g(-1, 0, 0).type == Block_Type::Water){
    //      Flow_Force n_ff = water_block_flow_force(N.g(-1, 0, 0));
    //      if (get<0>(n_ff) > 0){
    //        get<0>(computed_ff) += get<0>(n_ff);
    //      }
    //    }

    //    if (N.g(1, 0, 0).type == Block_Type::Water){
    //      Flow_Force n_ff = water_block_flow_force(N.g(1, 0, 0));
    //      if (get<0>(n_ff) < 0){
    //        get<0>(computed_ff) += get<0>(n_ff);
    //      }
    //    }

    //    if (N.g(0, -1, 0).type == Block_Type::Debug){
    //        if (rand() % 2)
    //          get<0>(computed_ff) += -get<1>(computed_ff);
    //        else
    //          get<0>(computed_ff) += get<1>(computed_ff);
    //        get<1>(computed_ff) = 0;
    //    }

    //    changes.insert({ xyz, mk_water_block(computed_ff) });

    //    //if (N.g(0, -1, 0).type == Block_Type::Empty){
    //    //  if (get<1>(ff) < 0){
    //    //    changes.insert({ xyz, Empty_Block });
    //    //  } else if (get<1>(ff) == 0){
    //    //    get<1>(ff) = -1;
    //    //    changes.insert({ xyz, mk_water_block(ff) });
    //    //  }
    //    //} else if (N.g(0, -1, 0).type == Block_Type::Debug){
    //    //    get<1>(ff) = 0;
    //    //    changes.insert({ xyz, mk_water_block(ff) });
    //    //}
    //  }
    //} else if (N.g(0, 0, 0).type == Block_Type::Empty){
    //  if (N.g(0, 1, 0).type == Block_Type::Water){
    //    Flow_Force ff = water_block_flow_force(N.g(0, 1, 0));
    //    std::tie(x, y, z) = ff;
    //    if (abs(y) > abs(x) && abs(y) > abs(z) && y < 0){
    //      changes.insert({ xyz, mk_water_block(Flow_Force(0, 0, 0)) });
    //    }
    //  } else if (N.g(-1, 0, 0).type == Block_Type::Water){
    //    Flow_Force ff = water_block_flow_force(N.g(-1, 0, 0));
    //    std::tie(x, y, z) = ff;
    //    if (abs(x) > abs(y) && abs(x) > abs(z) && x > 0){
    //      changes.insert({ xyz, mk_water_block(Flow_Force(0, 0, 0)) });
    //    }
    //  } else if (N.g(1, 0, 0).type == Block_Type::Water){
    //    Flow_Force ff = water_block_flow_force(N.g(1, 0, 0));
    //    std::tie(x, y, z) = ff;
    //    if (abs(x) > abs(y) && abs(x) > abs(z) && x < 0){
    //      changes.insert({ xyz, mk_water_block(Flow_Force(0, 0, 0)) });
    //    }
    //  }
    //}

    //if (N.g(0, 0, 0).type == Block_Type::Water){
    //  if (water_block_flow_dir(N.g(0, 0, 0)) == Flow_Dir::None){
    //    for (int i = 0; i < 5; i++){
    //      Flow_Dir d; int x; int z;
    //      tie(d, x, z) = flow_dirs[i];
    //      if (   (N.empty(x, 0, z) || (x == 0 && z == 0)) 
    //          && N.empty(x, -1, z)){
    //        changes.insert({ xyz, mk_water_block(d) });
    //        break;
    //      }
    //    }
    //  } else {
    //    for (int i = 0; i < 5; i++){
    //      Flow_Dir d; int x; int z;
    //      tie(d, x, z) = flow_dirs[i];
    //      if (water_block_flow_dir(N.g(0, 0, 0)) == d){
    //        if (   (N.empty(x, 0, z) || x == 0 && z == 0) 
    //            && N.empty(x, -1, z)){
    //          changes.insert({ xyz, Empty_Block });
    //        } else {
    //          changes.insert({ xyz, mk_water_block(Flow_Dir::None) });
    //        }
    //        break;
    //      }
    //    }
    //  }
    //} else if (N.g(0, 0, 0).type == Block_Type::Empty){
    //  for (int i = 0; i < 5; i++){
    //    Flow_Dir d; int x; int z;
    //    tie(d, x, z) = flow_dirs[i];
    //    if (   (N.empty(0, 1, 0) || (x == 0 && z == 0))
    //        && N.g(-x, 1, -z).type == Block_Type::Water
    //        && water_block_flow_dir(N.g(-x, 1, -z)) == d){
    //      add_water_block(changes, N);
    //      break;
    //    }
    //  }
    //}
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
