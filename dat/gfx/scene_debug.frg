#version 150
precision mediump float;


uniform sampler2D texture0;
uniform vec4 texture_location;
uniform vec4 color;

in vec2 tex_coord;
in float facing;
in float distance;

void main(void) {
  vec3 fog;
	fog.x = distance*0.005;
	fog.y = distance*0.005;
	fog.z = distance*0.005;

  vec2 coord =  tex_coord*texture_location.zw + texture_location.xy;
  gl_FragColor  = texture2D(texture0, coord);
  gl_FragColor.x *= facing-fog.x;
  gl_FragColor.y *= facing-fog.y;
  gl_FragColor.z *= facing-fog.z;
}

