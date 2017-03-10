
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
#include <iomanip>

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

  float fov = 45.0f;

  glm::mat4 projection = glm::perspective(fov, 1.0f*render_state.screen_width/render_state.screen_height, 0.1f, 500.0f);

  int drawn = 0;

  for (int i = 0; i < render_state.chunks.size(); i++){
    Chunk chunk = render_state.chunks[i];

    glm::vec3 axis_y(1, 0, 0);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunk.x, chunk.y, chunk.z));

    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(render_state.uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    int c_drawn = draw_chunk(chunk, render_state.player_x, render_state.player_y, render_state.player_z, 
                      render_state.player_rx, render_state.player_ry, render_state.player_rz, fov);
    drawn += c_drawn;
  }

  const long t = epoch_millis();

  double fps = 0;
  const int S = 30;
  if (render_state.last_draw_times.size() > S){
    render_state.last_draw_times.erase(render_state.last_draw_times.begin());
    fps = S*1000/(t*1. - render_state.last_draw_times[0]);
  } 

  cout << fixed << setprecision(1) << fps << " " << drawn << endl;

  render_state.last_draw_times.push_back(t);

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
