
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

#include <vector>

const int DISPLAY_TIMES_LEN = 100;

typedef struct State_ {
  long last_update_time;
  long queued_runtime;

  bool lock_pointer;

  int last_drag_x;
  int last_drag_y;

  float last_player_x;
  float last_player_y;
  float last_player_z;

  int step;

  World world;
} State;

typedef struct Display_Info_ {
  std::vector<long> last_draw_times;
  int tris;
  float fps;
  float render_time;
} Display_Info;

typedef struct Render_State_ {
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

  Cube cube;
  std::vector<Chunk> chunks;

} Render_State;

extern State state;
extern Display_Info display_info;
extern Render_State render_state;


