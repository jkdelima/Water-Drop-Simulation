#version 430 core

out vec4 color;

in vec3 N;
in vec3 V;
in vec3 L;

// Material properties
uniform vec3 diffuse_albedo = vec3(0.0,  1.0,  0.0); //blue
uniform vec3 specular_albedo = vec3(1.0); //white
uniform vec3 ambient_albedo = vec3(1.0);  //white


uniform float ka = 0.1;
uniform float kd = 0.5;
uniform float ks = 1.0;
uniform float ke = 5.0;

void main(void)
{    	

    // Calculate R locally
    vec3 R = reflect(-L, N);

    vec3 ambient =  ka * ambient_albedo;
    vec3 diffuse =  kd * max(dot(N, L), 0.0) * diffuse_albedo;
    vec3 specular = ks * pow(max(dot(R, V), 0.0), ke) * specular_albedo;

    color = vec4(ambient + diffuse + specular, 1.0);
}
