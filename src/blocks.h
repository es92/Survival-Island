
enum Block_Type { Empty, Debug, Water };

typedef struct Block_ {
  Block_Type type;
  short data[8];
} Block;

extern Block Empty_Block;

extern Block Debug_Block;

extern Block Water_Block;

Block mk_water_block(short water_level, bool on_ground);
bool water_block_on_ground(Block b);
short water_block_water_level(Block b);
