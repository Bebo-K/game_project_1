#version 150
precision mediump float;

//Skybox with no projection curvature. Similar to Mario 64.
//This shader only works with a unit plane covering (-1,-1) to (1,1)

uniform sampler2D texture0;
uniform vec4 texture_location;
uniform vec4 color;

in vec2 coords; //XY:  X coordinate + turn, Y coordinate + pitch

void main(void) {
  vec2 tex_coord = vec2(0,0);
	tex_coord.x = coords.x;
	tex_coord.y = coords.y;
	
  tex_coord.x = mod((tex_coord.x+1)/2,1);							//X: scale range [-1, 1] to [0, 1]. Wraps
  tex_coord.y = (tex_coord.y+1)/2;									//Y: scale range [-1, 1] to [0, 1].
  if(mod(floor(tex_coord.y),2) == 1){tex_coord.y= -tex_coord.y;}	//Y: mirror all ranges [2i+1,(2+1)i] to [-2i+1,-(2+1)i] (for sawtooth)
  tex_coord.y = mod(-tex_coord.y,1);								//Y: map all ranges to [0,1] and flips to match tex_coord range
  
  vec2 coord =  tex_coord*texture_location.zw + texture_location.xy;
  gl_FragColor  = texture2D(texture0, coord);
}

