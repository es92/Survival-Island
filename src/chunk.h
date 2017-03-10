#ifndef CHUNK_
#define CHUNK_

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

  GLuint ibo_chunk_x_pos_elements;
  GLuint ibo_chunk_x_neg_elements;
  GLuint ibo_chunk_y_pos_elements;
  GLuint ibo_chunk_y_neg_elements;
  GLuint ibo_chunk_z_pos_elements;
  GLuint ibo_chunk_z_neg_elements;

  GLint attribute_coord3d;
  GLint attribute_v_color;

  bool empty;
} Chunk;

bool init_chunk(Chunk& chunk, GLuint program, int cx, int cy, int cz, World& world);
void unload_chunk(Chunk& chunk);
int draw_chunk(glm::mat4 projection_view, Chunk& chunk, double x, double y, double z, double rx, double ry, double rz, float fov);

#endif
