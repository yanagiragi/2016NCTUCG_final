#version 440

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 proj_depth;
uniform mat4 view_depth;

layout(location = 0) in vec3 position;

out vec3 ShadowCoord;
mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
						0.0, 0.5, 0.0, 0.0,
						0.0, 0.0, 0.5, 0.0,
						0.5, 0.5, 0.5, 1.0);

void main() 
{
	gl_Position = proj * view * model * vec4(position, 1.0);
	ShadowCoord = vec3(biasMatrix *  proj_depth * view_depth * model * vec4(position, 1.0));
}