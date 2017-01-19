
#include <queue>

void on_mouse(int btn, int state, int x, int y);
void on_mouse_move(int x, int y);
void on_mouse_drag(int x, int y);
void on_reshape(int w, int h);
void on_key(unsigned char key, int x, int y);
void on_special_key(int key, int x, int y);
void on_key_up(unsigned char key, int x, int y);
void on_special_key_up(int key, int x, int y);


enum Events { Base, Mouse, Key, Reshape, Mouse_Move, Mouse_Drag, Special_Key, Special_Key_Up, Key_Up };

class Event {
  public:
    virtual Events type() { return Events::Base; }
};

class Key_Event : public Event {
  public:
    Events type() { return Events::Key; }
    unsigned char key;
    int x; 
    int y;
    Key_Event(unsigned char _key, int _x, int _y) : 
          key(_key),
          x(_y), 
          y(_x){ }
};

class Special_Key_Event : public Event {
  public:
    Events type() { return Events::Special_Key; }
    unsigned char key;
    int x; 
    int y;
    Special_Key_Event(unsigned char _key, int _x, int _y) : 
          key(_key),
          x(_y), 
          y(_x){ }
};

class Key_Up_Event : public Event {
  public:
    Events type() { return Events::Key_Up; }
    unsigned char key;
    int x; 
    int y;
    Key_Up_Event(unsigned char _key, int _x, int _y) : 
          key(_key),
          x(_y), 
          y(_x){ }
};

class Special_Key_Up_Event : public Event {
  public:
    Events type() { return Events::Special_Key_Up; }
    unsigned char key;
    int x; 
    int y;
    Special_Key_Up_Event(unsigned char _key, int _x, int _y) : 
          key(_key),
          x(_y), 
          y(_x){ }
};

class Mouse_Event : public Event {
  public:
    Events type() { return Events::Mouse; }
    int button;
    int button_state;
    int x;
    int y;
    Mouse_Event(int _button, int _button_state, int _x, int _y) : 
          button(_button), 
          button_state(_button_state),
          x(_x),
          y(_y){ }
};

class Reshape_Event : public Event {
  public:
    Events type() { return Events::Reshape; }
    int w;
    int h;
    Reshape_Event(int _w, int _h) : 
          w(_w), 
          h(_h){ }
};

class Mouse_Move_Event : public Event {
  public:
    Events type() { return Events::Mouse_Move; }
    int x;
    int y;
    Mouse_Move_Event(int _x, int _y) : 
          x(_x), 
          y(_y){ }
};

class Mouse_Drag_Event : public Event {
  public:
    Events type() { return Events::Mouse_Drag; }
    int x;
    int y;
    Mouse_Drag_Event(int _x, int _y) : 
          x(_x), 
          y(_y){ }
};

extern std::queue<Event*> event_queue;
