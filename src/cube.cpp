
#include "cube.h"

#include <iostream>
using namespace std;

void draw_cube(Cube& cube){
  glEnableVertexAttribArray(cube.attribute_coord3d);
  // Describe our vertices array to OpenGL (it can't guess its format automatically)
  glBindBuffer(GL_ARRAY_BUFFER, cube.vbo_cube_vertices);
  glVertexAttribPointer(
    cube.attribute_coord3d, // attribute
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  glEnableVertexAttribArray(cube.attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, cube.vbo_cube_colors);
  glVertexAttribPointer(
    cube.attribute_v_color, // attribute
    3,                 // number of elements per vertex, here (R,G,B)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  /* Push each element in buffer_vertices to the vertex shader */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ibo_cube_elements);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(cube.attribute_coord3d);
  glDisableVertexAttribArray(cube.attribute_v_color);
}

bool init_cube(Cube& cube, GLuint program){
  GLfloat cube_vertices[] = {
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
  };
  glGenBuffers(1, &cube.vbo_cube_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, cube.vbo_cube_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

  GLfloat cube_colors[] = {
    // front colors
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    // back colors
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
  };
  glGenBuffers(1, &cube.vbo_cube_colors);
  glBindBuffer(GL_ARRAY_BUFFER, cube.vbo_cube_colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

  GLushort cube_elements[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 0, 3,
    3, 7, 4,
    // left
    4, 5, 1,
    1, 0, 4,
    // right
    3, 2, 6,
    6, 7, 3,
  };
  glGenBuffers(1, &cube.ibo_cube_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.ibo_cube_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

  const char* attribute_name;
  attribute_name = "coord3d";
  cube.attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (cube.attribute_coord3d == -1) {
    cerr << "Could not bind attribute " << attribute_name << endl;
    return false;
  }
  attribute_name = "v_color";
  cube.attribute_v_color = glGetAttribLocation(program, attribute_name);
  if (cube.attribute_v_color == -1) {
    cerr << "Could not bind attribute " << attribute_name << endl;
    return false;
  }
  return true;
}
