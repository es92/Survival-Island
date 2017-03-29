
#include "world.h"

typedef struct Automata_ {

} Automata;

void step_automata(World& world, Automata& automata);

#include <boost/unordered_map.hpp>
#include <iostream>
using boost::unordered::unordered_map;

#include <vector>
#include "utils.h"
using std::vector;
using std::pair;

typedef std::function<bool(const Block&)> Block_Condition;

typedef vector<pair<XYZ, Block_Condition>> Block_Conditions;

enum Conditions { C_Base, Simple_And };

class Condition {
  public:
  virtual Conditions type() { return Conditions::C_Base; }
  virtual bool matches(Neighborhood& N){ return false; }
};

class Simple_And_Condition : public Condition {
  public:
  Conditions type() { return Conditions::Simple_And; }
  bool matches(Neighborhood& N);
  Simple_And_Condition(Block_Conditions _block_conditions) : block_conditions(_block_conditions){ }

  private:
  Block_Conditions block_conditions;
};

typedef struct Rule_ {
  Condition* condition;
  Block result;
} Rule;

extern unordered_map<Block_Type, vector<Rule> > rules;
