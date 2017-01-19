#include <GL/glut.h>

#include "display.h"
#include "main_loop.h"
#include "events.h"

int main(int argc, char **argv) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_SINGLE|GLUT_DEPTH);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(480,480);
    glutCreateWindow("Survival Island");

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

    glEnable(GL_DEPTH_TEST);

    init();

    glutMainLoop();
    return 0;
}

