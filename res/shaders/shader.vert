layout(location = 0) in vec4 aPos; //needs to be vec4 for matrix multiplication
layout(location = 1) in vec3 aNorm; 

//uniform float time;
uniform mat4 terrain_view;
uniform mat4 fixed_view;
uniform vec3 camera_pos;

out float lambertian;
out vec4 frag_pos;
out float fog_level;

uniform float freq_factor = 1.18;
uniform float amp_factor = .8;
uniform float init_amp_terrain = 10.;
uniform float init_freq_terrain = .01;

uniform float init_amp_coarse = .6;
uniform float init_freq_coarse = .1;

vec4 noise(float init_amp, float init_freq, vec4 pos, int iter) {
	float amp = init_amp;
	float freq = init_freq;
	float height = 0.;
	vec3 gradient = vec3(0.);

	for (int i = 0; i < iter; i++) {
		vec3 current_gradient = vec3(0.);
		height += amp * snoise(pos.xyz * freq, current_gradient);
		gradient += amp * freq * current_gradient;

		amp *= amp_factor;
		freq *= freq_factor;

		pos = pos + vec4(10234.23, -293.532, -212.321532, -5949.3);
	}

	return vec4(gradient, height);
}

int lod_mix_terrain (float dist) { //less detail far away
    return int(mix(12., 8., clamp(0., 1., dist/45.-4.)));
}

int lod_mix_coarse(float dist){
	return 4 - int(log2(dist)/2.);
}

float camera_height = noise(init_amp_terrain, init_freq_terrain, vec4(camera_pos, 1.), 16).w + 1.2;

void main(){
	
	vec4 terrain_pos = terrain_view * aPos + vec4(camera_pos, 0.);
	//find coordinates in terrain of player

	float dist = length(terrain_pos.xyz - camera_pos);

	int terrain_iter = lod_mix_terrain(dist);
	int coarse_iter = lod_mix_coarse(dist);

	vec4 terrain_vals = noise(init_amp_terrain, init_freq_terrain, terrain_pos, terrain_iter);
	vec4 coarse_vals = noise(init_amp_coarse, init_freq_coarse, terrain_pos, coarse_iter);

	float height = coarse_vals.w - camera_height;
	vec3 grad = terrain_vals.xyz + coarse_vals.xyz;
	//find height and gradient at that point

	frag_pos = fixed_view * aPos + vec4(0., height, 0., 0.);

	vec3 view_dir = normalize(-camera_pos);
	
	vec3 light_dir = normalize(vec3(1, 1, 1));
	
	fog_level = max(0., 1. - pow(.5, frag_pos.z / 20. - .5));

	lambertian = mix(.5, 1., dot(light_dir, grad));

	gl_Position = frag_pos; 
}

