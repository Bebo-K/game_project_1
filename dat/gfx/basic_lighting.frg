#version 150
precision mediump float;

uniform sampler2D texture0;
uniform vec4 texture_location;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

in vec2 tex_coord;
in vec3 normal;
in vec4 world_pos;

vec3 light_position = vec3(-10,10,-7);

void main(void) {

	vec3 lighting = vec3(0,0,0);
	vec3 light_direction = normalize(light_position-world_pos.xyz);


	lighting += ambient;
	
	float light_dot_normal = dot(light_direction,normal);
	if(light_dot_normal > 0.0){
		lighting += light_dot_normal*diffuse;
		lighting += specular*0.01;
	}
	
	vec2 coord =  tex_coord*texture_location.zw + texture_location.xy;
	gl_FragColor  = vec4(lighting,1.0)*texture2D(texture0, coord);
}

