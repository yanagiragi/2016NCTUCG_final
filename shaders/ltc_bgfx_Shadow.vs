#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 UV;
layout(location = 2) in vec3 a_normal;

uniform mat4 MVP;
uniform mat4 model;
uniform vec3 LightCenter;
uniform float roughness;	// bind roughness   {label:"Roughness", default:0.25, min:0.01, max:1, step:0.001}
uniform vec3 dcolor;		// bind dcolor      {label:"Diffuse Color",  r:1.0, g:1.0, b:1.0}
uniform vec3 scolor;		// bind scolor      {label:"Specular Color", r:1.0, g:1.0, b:1.0}
uniform float intensity;	// bind intensity   {label:"Light Intensity", default:4, min:0, max:10}
uniform float width;		// bind width       {label:"Width",  default: 8, min:0.1, max:15, step:0.1}
uniform float height;		// bind height      {label:"Height", default: 8, min:0.1, max:15, step:0.1}
uniform float roty;			// bind roty        {label:"Rotation Y", default: 0, min:0, max:1, step:0.001}
uniform float rotz;			// bind rotz        {label:"Rotation Z", default: 0, min:0, max:1, step:0.001}
uniform bool twoSided;		// bind twoSided    {label:"Two-sided", default:false}
uniform int mode;
uniform mat4 view;
uniform vec2 resolution;
uniform sampler2D ltc_mat;
uniform sampler2D ltc_mag;
uniform vec3 u_viewPosition;

uniform mat4 proj_depth;
uniform mat4 view_depth;

out vec3 normal;
out vec3 v_wpos;
out vec4 ShadowCoord;
mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
						0.0, 0.5, 0.0, 0.0,
						0.0, 0.0, 0.5, 0.0,
						0.5, 0.5, 0.5, 1.0);

void main() {
	gl_Position = MVP * vec4(position, 1.0);
	ShadowCoord = biasMatrix * proj_depth * view_depth * model * vec4(position, 1.0);

	vec3 tmpnormal = normalize(a_normal) * 2.0 - 1.0;
 	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	normal = normalize(normalMatrix  * tmpnormal.xyz);
	normal = a_normal;
	
	v_wpos = (model * vec4(position, 1.0)).xyz;
}