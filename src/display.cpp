
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

#include <vector>

using namespace std;

void display(void) {

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glUseProgram(render_state.program);

  glm::mat4 view = glm::rotate(glm::mat4(1.0f), glm::radians(render_state.player_rx), glm::vec3(1, 0, 0)) * 
                   glm::rotate(glm::mat4(1.0f), glm::radians(render_state.player_ry), glm::vec3(0, 1, 0)) * 
                   glm::rotate(glm::mat4(1.0f), glm::radians(render_state.player_rz), glm::vec3(0, 0, 1)) *
                   glm::translate(glm::mat4(1.0f), glm::vec3(render_state.player_x, render_state.player_y, render_state.player_z));

  glm::mat4 projection = glm::perspective(45.0f, 1.0f*render_state.screen_width/render_state.screen_height, 0.1f, 500.0f);

  for (int i = 0; i < render_state.chunks.size(); i++){
    Chunk chunk = render_state.chunks[i];
    glm::vec3 axis_y(1, 0, 0);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.x, chunk.y, chunk.z));

    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(render_state.uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    draw_chunk(chunk);
  }

  glutSwapBuffers();
}

void free_resources() {
  glDeleteProgram(render_state.program);
  glDeleteBuffers(1, &render_state.cube.vbo_cube_vertices);
  glDeleteBuffers(1, &render_state.cube.vbo_cube_colors);
  glDeleteBuffers(1, &render_state.cube.ibo_cube_elements);

  for (int i = 0; i < render_state.chunks.size(); i++){
    Chunk chunk = render_state.chunks[i];
    unload_chunk(chunk);
  }
}
