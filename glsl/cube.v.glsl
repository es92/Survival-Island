#version 330

attribute vec3 coord3d;
uniform mat4 mvp;
attribute vec2 v_tex_coord;
attribute float brightness;

varying vec2 tex_coords;
varying float frag_brightness;

void main(void) {
  gl_Position = mvp * vec4(coord3d, 1.0);
  frag_brightness = brightness;
  tex_coords = v_tex_coord;
}
