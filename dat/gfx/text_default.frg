#version 150
precision mediump float;

uniform sampler2D texture0;
uniform vec4 texture_location;

in vec2 tex_coord;

void main(void) {
	if (tex_coord.x >= 1.0 || tex_coord.y < 0.0)discard;
	vec2 coord =  tex_coord*texture_location.zw + texture_location.xy;
	gl_FragColor  = texture(texture0, coord);
}

