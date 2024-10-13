layout(location = 0) out vec4 color;

in vec4 frag_pos;
in float lambertian;
in float fog_level;

uniform vec3 light_color = vec3(1.0, 1.0, 1.0);
uniform float light_power = .5;

uniform vec3 ground_color = vec3(0.490, 0.88, 0.105) * 1.2;
uniform vec3 ambient_color = vec3(0.11, 0.5, 0.98);
uniform vec3 spec_color = vec3(.9, .9, 1.);
uniform float shininess = 6.;

uniform vec3 fog_color = vec3(0.45, 0.55, 0.6);

uniform float ambient_level = .1;

void main() {
  vec3 ambient = ambient_color;
  vec3 diffuse =  (ground_color * lambertian * light_color);
 // vec3 specular =  (spec_color * specular_value * light_color) ;

  vec3 colorLinear = mix(diffuse, ambient, ambient_level);

  colorLinear = colorLinear - colorLinear * fog_level + fog_color * (fog_level);

  gl_FragColor = vec4(colorLinear, 1.);
}
