
#include <GL/glut.h>
#include <vector>

const int DISPLAY_TIMES_LEN = 100;

typedef struct State_ {
  long last_update_time;
  long queued_runtime;
} State;

typedef struct Display_Info_ {
  std::vector<long> display_times;
} Display_Info;

typedef struct Render_State_ {
  GLfloat theta[3];
} Render_State;

extern State state;
extern Display_Info display_info;
extern Render_State render_state;
