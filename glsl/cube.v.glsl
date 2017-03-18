#version 330

attribute vec3 coord3d;
uniform mat4 mvp;
attribute vec2 v_tex_coord;
varying vec2 tex_coords;

void main(void) {
  gl_Position = mvp * vec4(coord3d, 1.0);
  tex_coords = v_tex_coord;
}
