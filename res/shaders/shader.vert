layout(location = 0) in vec4 aPos; //needs to be vec4 for matrix multiplication
layout(location = 1) in vec3 aNorm; 

//uniform float time;
uniform mat4 terrain_view;
uniform mat4 fixed_view;
uniform vec3 camera_pos;

out float lambertian;
out vec4 frag_pos;
out float fog_level;

void main(){
	
	vec4 terrain_pos = terrain_view * aPos + vec4(camera_pos, 0.);
	//find coordinates in terrain of player

	float dist = length(terrain_pos.xyz - camera_pos);

	float height = terrain_pos.y - camera_pos.y;
	vec3 grad = aNorm;
	//find height and gradient at that point

	frag_pos = fixed_view * aPos + vec4(0., height, 0., 0.);

	vec3 view_dir = normalize(-camera_pos);
	
	vec3 light_dir = normalize(vec3(1, 1, 1));
	
	fog_level = max(0., 1. - pow(.5, frag_pos.z / 20. - .5));

	lambertian = mix(.5, 1., dot(light_dir, grad));

	gl_Position = frag_pos; 
}

