#version 330

uniform sampler2D tex_cubes;

varying vec2 tex_coords;
varying float frag_brightness;

void main(void) {
  vec4 tex_color = texture2D(tex_cubes, vec2(tex_coords.x, tex_coords.y));
  float d = frag_brightness;
  gl_FragColor = vec4(tex_color.x*d, tex_color.y*d, tex_color.z*d, tex_color.w);
}
