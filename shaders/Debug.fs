#version 440

uniform vec3 debugColor;
uniform float Intensity;
out vec4 outColor;

void main() 
{
	outColor = vec4(debugColor , 1.0);
	// outColor = vec4(debugColor , 1.0) * Intensity;
}