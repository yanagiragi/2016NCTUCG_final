#version 440

uniform vec3 color;

out vec4 outColor;
in vec3 ShadowCoord;

void main() 
{
	//outColor = vec4(color, 1.0);
    if((gl_FragCoord.z < ShadowCoord.z) /*&& (color != vec3(0.0,1.0,0.0))*/){
		outColor = vec4(0.3,0.3,0.3,1.0);
		outColor.a = 0.5;
    }
    else
        outColor = vec4(color , 1.0); //vec4(vec3(pow(gl_FragCoord.z,10000)),1.0);
    //outColor = vec4(vec3(pow(gl_FragCoord.z,10000)),1.0);
    //outColor = vec4(color,1.0);
}