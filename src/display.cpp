

#include <GL/glut.h>
#include "display.h"
#include "state.h"
#include "utils.h"

void colorcube(void);
void polygon (int a, int b, int c, int d);

GLfloat vertices[][3] = {
  {-1.0, -1.0, -1.0},
  { 1.0, -1.0, -1.0},
  { 1.0,  1.0, -1.0},
  {-1.0,  1.0, -1.0},
  {-1.0, -1.0,  1.0},
  { 1.0, -1.0,  1.0},
  { 1.0,  1.0,  1.0},
  {-1.0,  1.0,  1.0}

};

GLfloat colors[][3] = {
  {0.0, 0.0, 0.0},
  {1.0, 0.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, 1.0},
  {1.0, 1.0, 0.0},
  {1.0, 0.0, 1.0},
  {0.0, 1.0, 1.0},
  {1.0, 1.0, 1.0}

};

void display(void) {

  display_info.display_times.push_back(epoch_millis());
  if (display_info.display_times.size() > DISPLAY_TIMES_LEN){
    display_info.display_times.erase(display_info.display_times.begin());
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glRotatef(render_state.theta[0], 1.0, 0.0, 0.0);
  glRotatef(render_state.theta[1], 0.0, 1.0, 0.0);
  glRotatef(render_state.theta[2], 0.0, 0.0, 1.0);

  colorcube();
  glFlush();          
  glutSwapBuffers();
}

void polygon (int a, int b, int c, int d) {
  glBegin(GL_POLYGON);

    glColor3fv(colors[a]);
    glVertex3fv(vertices[a]);

    glColor3fv(colors[b]);
    glVertex3fv(vertices[b]);

    glColor3fv(colors[c]);
    glVertex3fv(vertices[c]);

    glColor3fv(colors[d]);
    glVertex3fv(vertices[d]);

  glEnd();
}

void colorcube(void) {
  polygon(0, 3, 2, 1);
  polygon(2, 3, 7, 6);
  polygon(0, 4, 7, 3);
  polygon(1, 2, 6, 5);
  polygon(4, 5, 6, 7);
  polygon(0, 1, 5, 4);
}
