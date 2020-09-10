precision mediump float;
uniform sampler2D texture0;
uniform vec4 texture_location;
//uniform vec4 color;
varying vec2 tex_coord;
void main(void) {
  vec2 coord =  tex_coord*texture_location.zw + texture_location.xy;
  gl_FragColor  = texture2D(texture0, coord,0.0);
}

