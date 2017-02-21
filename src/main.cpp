#include <GL/glew.h>
#include <GL/glut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_utils.h"

#include "display.h"
#include "main_loop.h"
#include "events.h"

#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(480,480);
    glutCreateWindow("Survival Island -- dev");

    GLenum glew_status = glewInit();

    if (glew_status != GLEW_OK) {
      cerr << "Error: " << glewGetErrorString(glew_status) << endl;
      return 1;
    }

    if (!GLEW_VERSION_2_0) {
      cerr << "Error: your graphic card does not support OpenGL 2.0" << endl;
      return 1;
    }

    glutReshapeFunc(on_reshape);
    glutMouseFunc(on_mouse);
    glutMotionFunc(on_mouse_drag);
    glutPassiveMotionFunc(on_mouse_move);
    glutKeyboardFunc(on_key);
    glutKeyboardUpFunc(on_key_up);
    glutSpecialUpFunc(on_special_key_up);
    glutSpecialFunc(on_special_key);

    glutDisplayFunc(display);
    glutIdleFunc(main_loop);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (init()){
      glutMainLoop();
    }

    free_resources();

    return 0;
}

