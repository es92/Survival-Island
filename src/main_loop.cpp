

#include <GL/glew.h>
#include "main_loop.h"
#include <GL/glut.h>
#include "state.h"
#include "display.h"
#include "events.h"
#include "utils.h"

#include <iostream>
#include <set>
#include <algorithm>
#include <tuple>
#include <vector>

const int CHUNK_SIZE = 16;

using namespace std;

void step(void);
void process_events();
void process_event(Event* e);

std::set<unsigned char> down_keys;
std::set<int> down_special_keys;
std::set<int> down_mouse_buttons;

void set_chunks();

// ==========================================================

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
    render_state.angle -= 45*MILLIS_PER_UPDATE/1000.;
  }
  if (down_special_keys.find(GLUT_KEY_DOWN) != down_special_keys.end()) {
    render_state.angle += 45*MILLIS_PER_UPDATE/1000.;
  }
  if (down_special_keys.find(GLUT_KEY_RIGHT) != down_special_keys.end()) {
    render_state.angle += 45*MILLIS_PER_UPDATE/1000.;
  }
  if (down_special_keys.find(GLUT_KEY_LEFT) != down_special_keys.end()) {
    render_state.angle -= 45*MILLIS_PER_UPDATE/1000.;
  }

  render_state.angle += 5*MILLIS_PER_UPDATE/1000.;

  float walking_speed = 8;

  if (down_keys.find('c') != down_keys.end()) {
    walking_speed *= 10;
  }

  float cos_ang = cos(render_state.player_ry/180*M_PI);
  float sin_ang = sin(render_state.player_ry/180*M_PI);

  float sin_vert_ang = sin(render_state.player_rx/180*M_PI);
  float cos_vert_ang = cos(render_state.player_rx/180*M_PI);

  if (down_keys.find('a') != down_keys.end()) {
    render_state.player_x += cos_ang*walking_speed*MILLIS_PER_UPDATE/1000.;
    render_state.player_z += sin_ang*walking_speed*MILLIS_PER_UPDATE/1000.;
  }
  if (down_keys.find('s') != down_keys.end()) {
    render_state.player_x -= cos_ang*walking_speed*MILLIS_PER_UPDATE/1000.;
    render_state.player_z -= sin_ang*walking_speed*MILLIS_PER_UPDATE/1000.;
  }
  if (down_keys.find('w') != down_keys.end()) {
    render_state.player_x -= cos_vert_ang*sin_ang*walking_speed*MILLIS_PER_UPDATE/1000.;
    render_state.player_z += cos_vert_ang*cos_ang*walking_speed*MILLIS_PER_UPDATE/1000.;

    render_state.player_y += sin_vert_ang*walking_speed*MILLIS_PER_UPDATE/1000.;
  }
  if (down_keys.find('r') != down_keys.end()) {
    render_state.player_x += cos_vert_ang*sin_ang*walking_speed*MILLIS_PER_UPDATE/1000.;
    render_state.player_z -= cos_vert_ang*cos_ang*walking_speed*MILLIS_PER_UPDATE/1000.;

    render_state.player_y -= sin_vert_ang*walking_speed*MILLIS_PER_UPDATE/1000.;
  }


  set_chunks();
}

void set_chunks(){
  set<tuple<int, int, int> > active_chunks;
  int cx = 0;//render_state.player_x;
  int cy = 0;//render_state.player_y;
  int cz = 0;//render_state.player_z;
  cx = cx - cx % CHUNK_SIZE;
  cy = cy - cy % CHUNK_SIZE;
  cz = cz - cz % CHUNK_SIZE;

  int D = 3;

  for (int x = -D; x <= D; x++){
    for (int y = -D; y <= D; y++){
      for (int z = -D; z <= D; z++){
        active_chunks.insert(tuple<int, int, int>(-cx + x*CHUNK_SIZE, -cy + y*CHUNK_SIZE, -cz + z*CHUNK_SIZE));
      }
    }
  }

  set<tuple<int, int, int> > loaded_chunks;
  for (int i = 0; i < render_state.chunks.size(); i++){
    Chunk c = render_state.chunks[i];
    loaded_chunks.insert(tuple<int, int, int>(c.x, c.y, c.z));
  }

  set<tuple<int, int, int> > chunks_to_load;
  set_difference(active_chunks.begin(), active_chunks.end(), loaded_chunks.begin(), loaded_chunks.end(), inserter(chunks_to_load, chunks_to_load.end()));

  set<tuple<int, int, int> > xyzs_to_unload;
  set_difference(loaded_chunks.begin(), loaded_chunks.end(), active_chunks.begin(), active_chunks.end(), inserter(xyzs_to_unload, xyzs_to_unload.end()));

  vector<Chunk> chunks_to_unload;

  for (int i = 0; i < render_state.chunks.size(); i++){
    Chunk c = render_state.chunks[i];
    if (xyzs_to_unload.find(tuple<int, int, int>(c.x, c.y, c.z)) != xyzs_to_unload.end()){
      chunks_to_unload.push_back(c);
    }
  }

  for (int i = 0; i < chunks_to_unload.size(); i++){
    Chunk c = chunks_to_unload[i];
    unload_chunk(c);
    for (int j = 0; j < render_state.chunks.size(); j++){
      if (render_state.chunks[j].x == c.x && 
          render_state.chunks[j].y == c.y && 
          render_state.chunks[j].z == c.z){
        render_state.chunks.erase(render_state.chunks.begin() + j);
        break;
      }
    }
  }

  for (set<tuple<int, int, int> >::iterator it=chunks_to_load.begin(); it != chunks_to_load.end(); it++){
    int x, y, z;
    tie(x, y, z) = *it;
    cout << x << " " << y << " " << z << endl;
    Chunk chunk;
    if (!init_chunk(chunk, render_state.program, x, y, z, state.world)){
      throw runtime_error("could not init chunk");
    }
    render_state.chunks.push_back(chunk);

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

void trap_mouse(int x, int y) {
  if (!state.lock_pointer){
    return;
  }
  if (state.last_drag_x != -1){
    int diff_x = x - state.last_drag_x;
    int diff_y = y - state.last_drag_y;

    render_state.player_ry += diff_x/5.;
    render_state.player_rx += diff_y/5.;

    if (render_state.player_rx > 115)
      render_state.player_rx = 115;
    if (render_state.player_rx < -115)
      render_state.player_rx = -115;
  }

  int win_w = glutGet(GLUT_WINDOW_WIDTH);
  int win_h = glutGet(GLUT_WINDOW_HEIGHT);

  state.last_drag_x = win_w/2;
  state.last_drag_y = win_h/2;

  glutWarpPointer(win_w/2, win_h/2);
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

    trap_mouse(mme->x, mme->y);

  // ==============================================================
  } else if (e->type() == Events::Mouse_Drag) {
    Mouse_Drag_Event* mde = static_cast<Mouse_Drag_Event*>(e);

    trap_mouse(mde->x, mde->y);

  // ==============================================================
  } else if (e->type() == Events::Key) {
    Key_Event* ke = static_cast<Key_Event*>(e);
    down_keys.insert(ke->key);

    const int ESCAPE_KEY = 27;

    if (ke->key == ESCAPE_KEY){
      state.lock_pointer = !state.lock_pointer;
      if (state.lock_pointer){
        glutSetCursor(GLUT_CURSOR_NONE); 
      }
      else {
        state.last_drag_x = -1;
        state.last_drag_y = -1;
        glutSetCursor(GLUT_CURSOR_INHERIT); 
      }
    }

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

    render_state.screen_width = re->w;
    render_state.screen_height = re->h;
    glViewport(0, 0, render_state.screen_width, render_state.screen_height);

  // ==============================================================
  } else {
    cout << "dropped event" << e->type() << endl;
  }
}

bool init() {
  GLint link_ok = GL_FALSE;

  GLuint vs, fs;
  if ((vs = create_shader("../glsl/cube.v.glsl", GL_VERTEX_SHADER))   == 0) return false;
  if ((fs = create_shader("../glsl/cube.f.glsl", GL_FRAGMENT_SHADER)) == 0) return false;

  render_state.program = glCreateProgram();
  glAttachShader(render_state.program, vs);
  glAttachShader(render_state.program, fs);
  glLinkProgram(render_state.program);
  glGetProgramiv(render_state.program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    cerr << "glLinkProgram:";
    print_log(render_state.program);
    return false;
  }

  const char* uniform_name;
  uniform_name = "mvp";
  render_state.uniform_mvp = glGetUniformLocation(render_state.program, uniform_name);
  if (render_state.uniform_mvp == -1) {
    cerr << "Could not bind uniform " << uniform_name << endl;
    return false;
  }

  if (!init_cube(render_state.cube, render_state.program)){
    return false;
  }

  return true;
}

