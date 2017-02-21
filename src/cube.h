
#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"

typedef struct Cube_ {
  GLuint vbo_cube_vertices;
  GLuint vbo_cube_colors;
  GLuint ibo_cube_elements;
  GLint attribute_coord3d;
  GLint attribute_v_color;
} Cube;

bool init_cube(Cube& cube, GLuint program);
void draw_cube(Cube& cube);
