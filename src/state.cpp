
#include "state.h"
#include <GL/glut.h>

State state = {
  .last_update_time = -1,
  .queued_runtime = 0,
};

Render_State render_state = {
  .theta = { 0.0, 0.0, 0.0 },
};

Display_Info display_info = { };
