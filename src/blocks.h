
#include <tuple>
#include <array>

enum Block_Type { Empty, Debug, Water };

typedef struct Block_ {
  Block_Type type;
  short data[8];
} Block;

const short MAX_WATER_LEVEL = 16;

bool operator==(const Block& a, const Block& b);
bool operator!=(const Block& a, const Block& b);

extern Block Empty_Block;

extern Block Debug_Block;

extern Block Water_Block;

typedef std::array<short, 6> Flows;

Block mk_water_block(Flows flows, int level);
Block mk_water_block(Flows flows, int level);
Flows water_block_flows(const Block& b);
int water_block_level(const Block& b);


int get_block_tex_x(Block b);
int get_block_tex_y(Block b);
float get_block_height(Block b);
