#version 440

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout(location = 0) in vec3 position;

out vec4 pos;

void main() 
{
	pos = vec4(position, 1.0);
	gl_Position = proj * view * model * vec4(position, 1.0);
	//gl_Position = vec4(position, 1.0);
}