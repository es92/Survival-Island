
#include "automata.h"

using namespace std;


Block_Condition EMPTY = [](const Block& b) -> bool { return b.type == Block_Type::Empty; };
Block_Condition NOT_EMPTY = [](const Block& b) -> bool { return b.type != Block_Type::Empty; };

Block_Condition EMPTY_OR_WATER = [](const Block& b) -> bool { return b.type == Block_Type::Empty 
                                                                  || b.type == Block_Type::Water; };

Block_Condition WATER = [](const Block& b) -> bool { return b.type == Block_Type::Water; };

// ===========================================================

unordered_map<Block_Type, vector<Rule> > rules({
  { Block_Type::Water, { 

    Rule({ .condition = new Simple_And_Condition({
        { {0, -1, 0}, EMPTY },
        { {0, 0, 0}, [](const Block& b) -> bool { return get<1>(water_block_flow_force(b)) != -1; } },
       }), .result = mk_water_block({ 0, -1, 0 }) }),

    Rule({ .condition = new Simple_And_Condition({
        { {0, -1, 0}, EMPTY },
        { {0, 0, 0}, [](const Block& b) -> bool { return get<1>(water_block_flow_force(b)) == -1; } },
       }), .result = Empty_Block }),

    Rule({ .condition = new Simple_And_Condition({
        { {-1, 0, 0}, EMPTY },
        { {0, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) == -1; } },
      
       }), .result = Empty_Block }),

    Rule({ .condition = new Simple_And_Condition({
        { {1, 0, 0}, EMPTY_OR_WATER },
        { {0, 1, 0}, WATER },
        { {0, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) != 1; } },
      
       }), .result = mk_water_block({ 1, 0, 0 }) }),

    Rule({ .condition = new Simple_And_Condition({
        { {-1, 0, 0}, EMPTY_OR_WATER },
        { {0, 1, 0}, WATER },
        { {0, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) != 1; } },
      
       }), .result = mk_water_block({ -1, 0, 0 }) }),

    Rule({ .condition = new Simple_And_Condition({
        { {-1, 0, 0}, WATER },
        { {-1, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) == 1; } },
        { {0, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) != 1; } },
      
       }), .result = mk_water_block({ 1, 0, 0 }) }),

    Rule({ .condition = new Simple_And_Condition({
        { {1, 0, 0}, EMPTY },
        { {0, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) == 1; } },
      
       }), .result = Empty_Block }),

    Rule({ .condition = new Simple_And_Condition({
        { {1, 0, 0}, WATER },
        { {1, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) == -1; } },
        { {0, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) != -1; } },
      
       }), .result = mk_water_block({ -1, 0, 0 }) }),
    } },
  { Block_Type::Empty, { 
    Rule({ .condition = new Simple_And_Condition({
        { {0, 1, 0}, WATER },
        { {0, 1, 0}, [](const Block& b) -> bool { return get<1>(water_block_flow_force(b)) <= -1; } },
        { {0, -1, 0}, EMPTY },
       }), .result = mk_water_block({ 0, -1, 0 }) }),

    Rule({ .condition = new Simple_And_Condition({
        { {0, 1, 0}, WATER },
        { {0, 1, 0}, [](const Block& b) -> bool { return get<1>(water_block_flow_force(b)) <= -1; } },
        { {0, -1, 0}, NOT_EMPTY },
       }), .result = mk_water_block({ 0, 0, 0 }) }),

    Rule({ .condition = new Simple_And_Condition({
        { {-1, 0, 0}, WATER },
        { {-1, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) >= 1; } },
       }), .result = mk_water_block({ 0, 0, 0 }) }),

    Rule({ .condition = new Simple_And_Condition({
        { {1, 0, 0}, WATER },
        { {1, 0, 0}, [](const Block& b) -> bool { return get<0>(water_block_flow_force(b)) <= -1; } },
       }), .result = mk_water_block({ 0, 0, 0 }) }),

    } }
});
