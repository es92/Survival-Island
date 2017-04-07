

#include <GL/glew.h>
#include <GL/glut.h>

#include "state.h"

State state = {
  .last_update_time = -1,
  .queued_runtime = 0,
  .lock_pointer = false,
  .last_drag_x = -1,
  .last_drag_y = -1,
};

Render_State render_state = {
  .render_dist = 3,
  .angle = 0,
  .screen_width = 800,
  .screen_height = 600,
  .player_x = 0,
  .player_y = -16,
  .player_z = 0,
};

Display_Info display_info = { };

