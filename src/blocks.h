
#include <tuple>

enum Block_Type { Empty, Debug, Water };

typedef struct Block_ {
  Block_Type type;
  short data[8];
} Block;

extern Block Empty_Block;

extern Block Debug_Block;

extern Block Water_Block;

//enum Flow_Dir { None, Y_Neg, X_Pos, X_Neg, Z_Pos, Z_Neg };
//
//Block mk_water_block(Flow_Dir flow_dir);
//Flow_Dir water_block_flow_dir(Block b);


typedef std::tuple<short, short, short> Flow_Force;

Block mk_water_block(Flow_Force flow_force);
Flow_Force water_block_flow_force(Block b);

bool operator==(const Block& a, const Block& b);
bool operator!=(const Block& a, const Block& b);
