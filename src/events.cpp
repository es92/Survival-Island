
#include "events.h"

std::queue<Event*> event_queue;

void on_mouse(int button, int button_state, int x, int y) {
  event_queue.push(new Mouse_Event(button, button_state, x, y));
}

void on_reshape(int w, int h) {
  event_queue.push(new Reshape_Event(w, h));
}

void on_key(unsigned char key, int x, int y) {
  event_queue.push(new Key_Event(key, x, y));
}

void on_mouse_move(int x, int y) {
  event_queue.push(new Mouse_Move_Event(x, y));
}

void on_mouse_drag(int x, int y) {
  event_queue.push(new Mouse_Drag_Event(x, y));
}

void on_special_key(int key, int x, int y){
  event_queue.push(new Special_Key_Event(key, x, y));
}

void on_special_key_up(int key, int x, int y){
  event_queue.push(new Special_Key_Up_Event(key, x, y));
}

void on_key_up(unsigned char key, int x, int y){
  event_queue.push(new Key_Up_Event(key, x, y));
}
