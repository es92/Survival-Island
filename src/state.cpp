

#include <GL/glew.h>
#include <GL/glut.h>

#include "state.h"

State state = {
  .last_update_time = -1,
  .queued_runtime = 0,
};

Render_State render_state = {
  .angle = 0,
  .screen_width = 800,
  .screen_height = 600,
};

Display_Info display_info = { };

