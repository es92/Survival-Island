#ifndef STATE_
#define STATE_

#include <GL/glut.h>
#include <vector>
#include "cube.h"
#include "chunk.h"
#include "world.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"
#include "utils.h"
#include "automata.h"

#include "chunk_loader.h"
#include "async_chunk_action_stream.h"

#include <boost/unordered_map.hpp>
using  boost::unordered::unordered_map;

#include <boost/lockfree/spsc_queue.hpp>
using boost::lockfree::spsc_queue;

typedef struct State_ {
  long last_update_time;
  long queued_runtime;

  bool lock_pointer;

  int last_drag_x;
  int last_drag_y;

  float last_player_x;
  float last_player_y;
  float last_player_z;

  Async_Chunk_Action_Stream chunk_action_stream;

  int step;

  World world;
  Automata automata;
} State;

typedef struct Display_Info_ {
  std::vector<long> last_draw_times;
  int tris;
  float fps;
  float render_time;
} Display_Info;

typedef struct Render_State_ {
  int render_dist;
  float angle;
  int screen_width;
  int screen_height;

  float player_x;
  float player_y;
  float player_z;

  float player_rx;
  float player_ry;
  float player_rz;

  GLuint program;

  GLint uniform_mvp;

  unordered_map<XYZ, Chunk*> chunks;

  GLuint cubes_texture;

} Render_State;

extern State state;
extern Display_Info display_info;
extern Render_State render_state;

#endif

