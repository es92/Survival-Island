#ifndef CHUNK_
#define CHUNK_

#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utils/shader_utils.h"

#include "../world/world.h"

#include <vector>
using namespace std;

typedef struct Faces_ {
  short unsigned int index;

  vector<GLushort> elements;
  vector<GLfloat> vertices;
  vector<GLfloat> tex_coords;
  vector<GLfloat> brightness;

  GLuint ibo_elements;
  GLuint vbo_vertices;
  GLuint vbo_tex_coords;
  GLuint vbo_brightness;

  bool empty;

} Faces;

typedef struct Chunk_ {
  double x, y, z;

  Faces x_pos;
  Faces x_neg;
  Faces y_pos;
  Faces y_neg;
  Faces z_pos;
  Faces z_neg;

  GLint attribute_coord3d;
  GLint attribute_v_tex_coord;
  GLint attribute_brightness;

  bool empty;
} Chunk;

bool init_chunk_gl(Chunk& chunk, GLuint program, int cx, int cy, int cz, World& world);
void init_chunk_cubes(Chunk& chunk, int cx, int cy, int cz, World& world);
bool init_chunk(Chunk& chunk, GLuint program, int cx, int cy, int cz, World& world);
void unload_chunk(Chunk* chunk);
int draw_chunk(glm::mat4 projection_view, Chunk& chunk, double x, double y, double z, double rx, double ry, double rz, float fov);

#endif
