#version 440

uniform vec3 color;
uniform sampler2D shadowMap;

out vec4 outColor;
in vec3 ShadowCoord;

void main() 
{
	float visibility = 1.0;
	if ( texture( shadowMap, ShadowCoord.xy ).z  <  ShadowCoord.z){
		visibility = 0.5;
	}
	
	
    /*
	if((gl_FragCoord.z < ShadowCoord.z)){
		outColor = vec4(0.3,0.3,0.3,1.0);
		outColor.a = 0.5;
    }
    else
        outColor = vec4(color , 1.0); //vec4(vec3(pow(gl_FragCoord.z,10000)),1.0);	
	*/

	if((gl_FragCoord.z < ShadowCoord.z)){
		outColor = vec4(0.3,0.3,0.3,1.0);
		outColor.a = 0.5;
    }
    else
        outColor = vec4(color , 1.0); //vec4(vec3(pow(gl_FragCoord.z,10000)),1.0);	
    
}