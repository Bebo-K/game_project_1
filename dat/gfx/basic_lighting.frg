#version 150
precision mediump float;

uniform sampler2D texture0;
uniform vec4 texture_location;
uniform vec4 base_color;
vec3 ambient = vec3(1.0,1.0,1.0);
vec3 diffuse = vec3(1.0,1.0,1.0);
vec3 specular = vec3(1.0,1.0,1.0);


in vec4 color;
in vec2 tex_coord;
in vec3 normal;
in vec4 world_pos;

vec3 light_position = vec3(-10,10,-7);

void main(void) {

	/*
	vec3 lighting = vec3(0,0,0);
	vec3 light_direction = normalize(light_position-world_pos.xyz);

	lighting += ambient;
	
	float light_dot_normal = dot(light_direction,normal);
	if(light_dot_normal > 0.0){
		lighting += light_dot_normal*diffuse;
		lighting += light_dot_normal*light_dot_normal*specular;
	}
	
	output = vec4(lighting,1);
	*/
	
    float inv_gamma = 1.0/2.2;
	vec3 gamma_correct_basecolor = pow(base_color.xyz, vec3(inv_gamma));
	vec4 basecolor = vec4(gamma_correct_basecolor.xyz,base_color.w);
	
	vec2 coord =  tex_coord*texture_location.zw + texture_location.xy;
	gl_FragColor  = color*basecolor*texture2D(texture0, coord);
}

