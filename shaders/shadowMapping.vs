#version 440

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 proj_depth;
uniform mat4 view_depth;
uniform mat4 DepthBiasMVP;
uniform vec3 LightInvDirection_worldspace;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

out vec2 UV;
out vec3 Normal_ObjectSpace;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

out vec4 ShadowCoord;
mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
						0.0, 0.5, 0.0, 0.0,
						0.0, 0.0, 0.5, 0.0,
						0.5, 0.5, 0.5, 1.0);

void main() 
{
	gl_Position = proj * view * model * vec4(position, 1.0);
	
	ShadowCoord = biasMatrix * proj_depth * view_depth * model * vec4(position, 1.0);
	
	Position_worldspace = (model * vec4(position, 1)).xyz;
	
	EyeDirection_cameraspace = vec3(0,0,0) - ( view * model * vec4(position, 1)).xyz;

	LightDirection_cameraspace = (view * vec4(LightInvDirection_worldspace,0)).xyz;
	
	// Normal of the the vertex, in camera space
	//Normal_cameraspace = mat3(transpose(inverse(view * model))) * vertexNormal_modelspace; 
	Normal_cameraspace = ( view * model * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	Normal_ObjectSpace = vertexNormal_modelspace;

	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}