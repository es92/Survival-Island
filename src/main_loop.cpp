

#include <GL/glew.h>
#include "main_loop.h"
#include <GL/glut.h>
#include "state.h"
#include "display.h"
#include "events.h"
#include "utils.h"
#include "timer.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <tuple>
#include <vector>
#include <boost/unordered_set.hpp>
using boost::unordered_set;

#include <chrono>
#include <thread>

const int CHUNK_SIZE = 16;
const int RENDER_DIST = 4;

using namespace std;

void step(void);
void process_events();
void process_event(Event* e);

unordered_set<unsigned char> down_keys;
unordered_set<int> down_special_keys;
unordered_set<int> down_mouse_buttons;

void set_chunks();
void load_all_chunks();

int snap_to_chunk(int x){
  return x - x % CHUNK_SIZE;
}

// ==========================================================

void main_loop() {
  const long t = epoch_millis();
  if (state.last_update_time == -1){
    state.last_update_time = t;
  }
  const long diff = t - state.last_update_time;

  state.queued_runtime += diff;

  while (state.queued_runtime > MILLIS_PER_UPDATE) {
    Timer timer;
    step();
    float tt = timer.end()*1000;
    if (tt > 5)
      cout << "ST " << fixed << setprecision(1) << tt << " RT " << display_info.render_time << " FPS " << display_info.fps << " TRIS " << display_info.tris << " CHUNKS " << render_state.chunks.size() << endl;
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

  if (
      state.step == 0 ||
      (snap_to_chunk(state.last_player_x) != snap_to_chunk(render_state.player_x)) ||
      (snap_to_chunk(state.last_player_y) != snap_to_chunk(render_state.player_y)) ||
      (snap_to_chunk(state.last_player_z) != snap_to_chunk(render_state.player_z))){
    //int d = set_chunks();
    if (state.step == 0){
      load_all_chunks();
    } else {
      set_chunks();
    }
    //if (state.step > 0 && d > 0){
    //  std::this_thread::sleep_for(std::chrono::milliseconds(120*1000));
    //}
  }


  state.last_player_x = render_state.player_x;
  state.last_player_y = render_state.player_y;
  state.last_player_z = render_state.player_z;


  state.step += 1;
}

void load_all_chunks(){
  int pcx = -snap_to_chunk(render_state.player_x);
  int pcy = -snap_to_chunk(render_state.player_y);
  int pcz = -snap_to_chunk(render_state.player_x);

  unordered_set<XYZ> xyzs_to_load;
  for (int x = -RENDER_DIST; x <= RENDER_DIST; x++){
    for (int y = -RENDER_DIST; y <= RENDER_DIST; y++){
      for (int z = -RENDER_DIST; z <= RENDER_DIST; z++){
        xyzs_to_load.insert(XYZ(-pcx + x*CHUNK_SIZE, -pcy + y*CHUNK_SIZE, -pcz + z*CHUNK_SIZE));
      }
    }
  }

  for (unordered_set<XYZ>::iterator it=xyzs_to_load.begin(); it != xyzs_to_load.end(); it++){
    int x, y, z;
    tie(x, y, z) = *it;
    Chunk chunk;
    if (!init_chunk(chunk, render_state.program, x, y, z, state.world)){
      throw runtime_error("could not init chunk");
    }
    render_state.chunks.insert({*it, chunk});
  }
}

void set_chunks(){
  int pcx = -snap_to_chunk(render_state.player_x);
  int pcy = -snap_to_chunk(render_state.player_y);
  int pcz = -snap_to_chunk(render_state.player_z);
  int lcx = -snap_to_chunk(state.last_player_x);
  int lcy = -snap_to_chunk(state.last_player_y);
  int lcz = -snap_to_chunk(state.last_player_z);

  unordered_set<XYZ> xyzs_to_unload;
  unordered_set<XYZ> xyzs_to_load;
  if (pcx != lcx){
    int load_cx = pcx + (pcx - lcx)*RENDER_DIST;
    int unload_cx = pcx + (lcx - pcx)*(RENDER_DIST+1);
    for (int y = -RENDER_DIST; y <= RENDER_DIST; y++){
      for (int z = -RENDER_DIST; z <= RENDER_DIST; z++){
        int cy = pcy + y*CHUNK_SIZE;
        int cz = pcz + z*CHUNK_SIZE;
        xyzs_to_load.insert(XYZ(load_cx, cy, cz));
        xyzs_to_unload.insert(XYZ(unload_cx, cy, cz));
      }
    }
  }
  if (pcy != lcy){
    int load_cy = pcy + (pcy - lcy)*RENDER_DIST;
    int unload_cy = pcy + (lcy - pcy)*(RENDER_DIST+1);
    for (int x = -RENDER_DIST; x <= RENDER_DIST; x++){
      for (int z = -RENDER_DIST; z <= RENDER_DIST; z++){
        int cx = pcx + x*CHUNK_SIZE;
        int cz = pcz + z*CHUNK_SIZE;
        xyzs_to_load.insert(XYZ(cx, load_cy, cz));
        xyzs_to_unload.insert(XYZ(cx, unload_cy, cz));
      }
    }
  }
  if (pcz != lcz){
    int load_cz = pcz + (pcz - lcz)*RENDER_DIST;
    int unload_cz = pcz + (lcz - pcz)*(RENDER_DIST+1);
    for (int y = -RENDER_DIST; y <= RENDER_DIST; y++){
      for (int x = -RENDER_DIST; x <= RENDER_DIST; x++){
        int cy = pcy + y*CHUNK_SIZE;
        int cx = pcx + x*CHUNK_SIZE;
        xyzs_to_load.insert(XYZ(cx, cy, load_cz));
        xyzs_to_unload.insert(XYZ(cx, cy, unload_cz));
      }
    }
  }

  for (unordered_set<XYZ>::iterator it=xyzs_to_unload.begin(); it != xyzs_to_unload.end(); it++){
    int x, y, z;
    tie(x, y, z) = *it;
    Chunk& c = render_state.chunks.find(*it)->second;
    unload_chunk(c);
    render_state.chunks.erase(render_state.chunks.find(*it)->first);
  }

  for (unordered_set<XYZ>::iterator it=xyzs_to_load.begin(); it != xyzs_to_load.end(); it++){
    int x, y, z;
    tie(x, y, z) = *it;
    Chunk chunk;
    //cout << "INSERT " << x << " "<< y << " " << z << endl;
    if (!init_chunk(chunk, render_state.program, x, y, z, state.world)){
      throw runtime_error("could not init chunk");
    }
    render_state.chunks.insert({*it, chunk});
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
  if (x == render_state.screen_width/2 && y == render_state.screen_height/2){
    state.last_drag_x = x;
    state.last_drag_y = y;
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

  if (   x < render_state.screen_width/10 || x > render_state.screen_width*9/10
      || y < render_state.screen_height/10 || y > render_state.screen_height*9/10){
    glutWarpPointer(render_state.screen_width/2, render_state.screen_height/2);
  } else {
    state.last_drag_x = x;
    state.last_drag_y = y;
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

