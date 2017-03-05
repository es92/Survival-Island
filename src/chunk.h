
#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"

#include "world.h"

typedef struct Chunk_ {
  double x, y, z;
  GLuint vbo_chunk_vertices;
  GLuint vbo_chunk_colors;
  GLuint ibo_chunk_elements;
  GLint attribute_coord3d;
  GLint attribute_v_color;
} Chunk;

bool init_chunk(Chunk& chunk, GLuint program, int cx, int cy, int cz, World& world);
void draw_chunk(Chunk& chunk);
