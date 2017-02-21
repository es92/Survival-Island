
#include <GL/glut.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"

const int DISPLAY_TIMES_LEN = 100;

typedef struct State_ {
  long last_update_time;
  long queued_runtime;
} State;

typedef struct Display_Info_ {
  std::vector<long> display_times;
} Display_Info;

typedef struct Render_State_ {
  float angle;
  int screen_width;
  int screen_height;
  GLuint program;
  GLint uniform_mvp;

  GLuint vbo_cube_vertices;
  GLuint vbo_cube_colors;
  GLuint ibo_cube_elements;
  GLint attribute_coord3d;
  GLint attribute_v_color;

} Render_State;

extern State state;
extern Display_Info display_info;
extern Render_State render_state;

