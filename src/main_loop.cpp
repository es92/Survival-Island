
#include "main_loop.h"
#include <GL/glut.h>
#include "state.h"
#include "display.h"
#include "events.h"
#include "utils.h"

#include <iostream>
#include <set>

using namespace std;

void step(void);
void process_events();
void process_event(Event* e);

std::set<unsigned char> down_keys;
std::set<int> down_special_keys;
std::set<int> down_mouse_buttons;

// ==========================================================

void init(void) {

}

void main_loop() {
  const long t = epoch_millis();
  if (state.last_update_time == -1){
    state.last_update_time = t;
  }
  const long diff = t - state.last_update_time;

  state.queued_runtime += diff;

  while (state.queued_runtime > MILLIS_PER_UPDATE) {
    step();
    state.queued_runtime -= MILLIS_PER_UPDATE;
    state.last_update_time = t;
  }
  glutPostRedisplay();
}

// ==========================================================

void step() {
  process_events();

  if (down_special_keys.find(GLUT_KEY_UP) != down_special_keys.end()) {
    render_state.theta[0] -= 0.5;
  }
  if (down_special_keys.find(GLUT_KEY_DOWN) != down_special_keys.end()) {
    render_state.theta[0] += 0.5;
  }
  if (down_special_keys.find(GLUT_KEY_RIGHT) != down_special_keys.end()) {
    render_state.theta[1] += 0.5;
  }
  if (down_special_keys.find(GLUT_KEY_LEFT) != down_special_keys.end()) {
    render_state.theta[1] -= 0.5;
  }
}

void process_events() {
  while (!event_queue.empty()) {
    Event* e = event_queue.front();
    event_queue.pop();
    process_event(e);
    delete e;
  }
}

void process_event(Event* e) {
  // ==============================================================
  if (e->type() == Events::Mouse) {
    Mouse_Event* me = static_cast<Mouse_Event*>(e);
    if (me->button_state == GLUT_DOWN) {
      down_mouse_buttons.insert(me->button);
    } else if (me->button_state == GLUT_UP) {
      down_mouse_buttons.erase(me->button);
    }
  // ==============================================================
  } else if (e->type() == Events::Mouse_Move) {
    Mouse_Move_Event* mme = static_cast<Mouse_Move_Event*>(e);
  // ==============================================================
  } else if (e->type() == Events::Mouse_Drag) {
    Mouse_Drag_Event* mde = static_cast<Mouse_Drag_Event*>(e);
  // ==============================================================
  } else if (e->type() == Events::Key) {
    Key_Event* ke = static_cast<Key_Event*>(e);
    down_keys.insert(ke->key);
  // ==============================================================
  } else if (e->type() == Events::Key_Up) {
    Key_Up_Event* ke = static_cast<Key_Up_Event*>(e);
    down_keys.erase(ke->key);
  // ==============================================================
  } else if (e->type() == Events::Special_Key) {
    Special_Key_Event* ske = static_cast<Special_Key_Event*>(e);
    down_special_keys.insert(ske->key);
  // ==============================================================
  } else if (e->type() == Events::Special_Key_Up) {
    Special_Key_Up_Event* ske = static_cast<Special_Key_Up_Event*>(e);
    down_special_keys.erase(ske->key);
  // ==============================================================
  } else if (e->type() == Events::Reshape) {
    Reshape_Event* re = static_cast<Reshape_Event*>(e);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if( re->w <= re->h){
      glOrtho( -2.0, 2.0, -2.0*((GLfloat)re->h/(GLfloat)re->w), 2.0*((GLfloat)re->h/(GLfloat)re->w), -10.0, 10.0);
    } else {
      glOrtho( -2.0*((GLfloat)re->w/(GLfloat)re->h), 2.0*((GLfloat)re->w/(GLfloat)re->h), -2.0, 2.0, -10.0, 10.0);
    }
    glMatrixMode(GL_MODELVIEW);  
  // ==============================================================
  } else {
    cout << "dropped event" << e->type() << endl;
  }
}
