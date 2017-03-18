#version 330

uniform sampler2D tex_cubes;

varying vec2 tex_coords;

void main(void) {
  gl_FragColor = texture2D(tex_cubes, vec2(tex_coords.x, tex_coords.y));
}
