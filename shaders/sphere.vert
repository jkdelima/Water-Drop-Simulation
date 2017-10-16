#version 430 core

uniform mat4 modelview;
uniform mat4 projection;
uniform vec3 lightPosition;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;

//attributes in camera coordinates
out vec3 N; //normal
out vec3 V; //view vector
out vec3 L; //light
out vec3 reflectV;
out vec3 refractV;
out vec2 UV;

void main(void)
{
    vec4 lightCameraSpace = modelview * vec4(lightPosition, 1.0);
    lightCameraSpace /= lightCameraSpace.w;

    mat3 normalMatrix = mat3(transpose(inverse(modelview)));
    N = normalize(normalMatrix * normal);

    float scale = 4.0;    
    vec3 positionModelSpace = vertex*scale + vec3(0.0, -0.42, 0.0);
    vec4 positionCameraSpace = modelview * vec4(positionModelSpace, 1.0);

    vec3 P = positionCameraSpace.xyz/positionCameraSpace.w;
   
    L = normalize(lightCameraSpace.xyz - P);
    V = normalize(-P);

    gl_Position = projection * positionCameraSpace;

	reflectV = reflect(V, N);
	refractV = refract(V, N, 0.75);

	float m = 2 * ( pow ( pow(reflectV.x, 2) + pow(reflectV.y, 2) + pow(reflectV.z + 1, 2), 0.5 ) );
	UV = vec2( (reflectV.x/m) + 0.5, (reflectV.y/m) + 0.5);
}
