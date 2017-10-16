#version 430 core

out vec4 color;

in vec3 N;
in vec3 V;
in vec3 L;
in vec3 reflectV;
in vec3 refractV;
in vec2 UV;

uniform sampler2D image;
uniform float refraction;

void main(void)
{    	
	vec3 R = normalize(reflect(-L, N));
	float diffuse = max(0, dot(N, L));
	vec4 colorImage = texture(image, UV);	
	vec3 refracted = vec3(-V * refractV);
	color = mix(colorImage + diffuse, vec4(refracted, 0), refraction);
}
