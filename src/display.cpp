
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
/* GLM */
// #define GLM_MESSAGES
#include <GL/glut.h>

#include "display.h"
#include "state.h"
#include "utils.h"

#include <fstream>
#include <iostream>

using namespace std;

void display(void) {

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glUseProgram(render_state.program);
  glEnableVertexAttribArray(render_state.attribute_coord3d);
  // Describe our vertices array to OpenGL (it can't guess its format automatically)
  glBindBuffer(GL_ARRAY_BUFFER, render_state.vbo_cube_vertices);
  glVertexAttribPointer(
    render_state.attribute_coord3d, // attribute
    3,                 // number of elements per vertex, here (x,y,z)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  glEnableVertexAttribArray(render_state.attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, render_state.vbo_cube_colors);
  glVertexAttribPointer(
    render_state.attribute_v_color, // attribute
    3,                 // number of elements per vertex, here (R,G,B)
    GL_FLOAT,          // the type of each element
    GL_FALSE,          // take our values as-is
    0,                 // no extra data between each position
    0                  // offset of first element
  );

  /* Push each element in buffer_vertices to the vertex shader */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_state.ibo_cube_elements);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(render_state.attribute_coord3d);
  glDisableVertexAttribArray(render_state.attribute_v_color);
  glutSwapBuffers();
}

void free_resources() {
  glDeleteProgram(render_state.program);
  glDeleteBuffers(1, &render_state.vbo_cube_vertices);
  glDeleteBuffers(1, &render_state.vbo_cube_colors);
  glDeleteBuffers(1, &render_state.ibo_cube_elements);
}
