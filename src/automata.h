
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
//typedef std::function<bool(const Neighborhood&)> Neighborhood_Condition;
//
//typedef std::function<Block(const Neighborhood&)> Block_Result;
//
//typedef vector<pair<XYZ, Block_Condition>> Block_Conditions;
//
//enum Conditions { C_Base, Simple_And, Custom };
//
//class Condition {
//  public:
//  virtual Conditions type() { return Conditions::C_Base; }
//  virtual bool matches(Neighborhood& N){ return false; }
//};
//
//class Simple_And_Condition : public Condition {
//  public:
//  Conditions type() { return Conditions::Simple_And; }
//  bool matches(Neighborhood& N);
//  Simple_And_Condition(Block_Conditions _block_conditions) : block_conditions(_block_conditions){ }
//
//  private:
//  Block_Conditions block_conditions;
//};
//
//class Custom_Condition : public Condition {
//  public:
//  Conditions type() { return Conditions::Custom; }
//  bool matches(Neighborhood& N) const { return nc(N); }
//  Custom_Condition(Neighborhood_Condition _nc) : nc(_nc){ }
//
//  private:
//  Neighborhood_Condition nc;
//};
//
//typedef struct Rule_ {
//  Condition* condition;
//  Block_Result result;
//} Rule;

typedef std::function<Block*(const Neighborhood& N)> Block_Rule;

extern unordered_map<Block_Type, vector<Block_Rule> > rules;
