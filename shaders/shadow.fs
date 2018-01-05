#version 440

uniform vec3 debugColor;
uniform sampler2DShadow shadowMap;
//uniform sampler2D shadowMap;
uniform float bias;

out vec4 outColor;
//in vec3 ShadowCoord;
in vec4 ShadowCoord;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

void main() 
{
	/*
	if((gl_FragCoord.z < ShadowCoord.z))
	{
		outColor = vec4(0.3,0.3,0.3,1.0);
		outColor.a = 0.5;
    }
    else
	{
        outColor = vec4(color , 1.0); //vec4(vec3(pow(gl_FragCoord.z,10000)),1.0);	
	}
	*/

	//vec4 depthVec4 = texture( shadowMap, ShadowCoord.xy );

	//float depth = depthVec4.z / depthVec4.w;

	// depth = (depth + 1.0) / 2.0;

	/*if ( depth  <  ShadowCoord.z )
	{
		outColor = vec4(0.3,0.3,0.3,1.0);
    }
    else
	{
	    outColor = vec4(color , 1.0); //vec4(vec3(pow(gl_FragCoord.z,10000)),1.0);	
	}
    
	if(depth == 1)
	{
		
	    //outColor = vec4(color , 1.0); //vec4(vec3(pow(gl_FragCoord.z,10000)),1.0);	
		//outColor = vec4(vec3(pow(depth,10000)),1.0);	
	}*/

	float visibility=1.0;
	float bias = 0.008;

	for (int i=0; i<4; i++)
	{
		int index = i;
		
		// 0.2 potentially remain, which is quite dark.
		vec2 UVxy = ShadowCoord.xy + poissonDisk[index]/700.0;
		vec3 uv = vec3(
			UVxy.x,
			UVxy.y,
			(ShadowCoord.z-bias)/ShadowCoord.w
		);
		visibility -= 0.2 * ( 1.0 - texture( shadowMap, uv ));
	}

	outColor = vec4(debugColor , 1.0) * visibility;
	
	//vec2 UVxy = ShadowCoord.xy;
	//vec3 uv = vec3(
	//	(1- UVxy.x) / 2.0,
	//	(1- UVxy.y) / 2.0,
	//	(1 - (ShadowCoord.z-bias)/ShadowCoord.w) / 2.0
	//);

	//float d  = 1.0 - texture( shadowMap, vec3(ShadowCoord.xy , uv));

	//d  = 1 - ShadowCoord.z;
	//d /= 2.0;
	//outColor = vec4(d, d, d , 1.0);
	
	///*if(d > 0.5){
	//	outColor = vec4(1, 0, 0, 1.0);
	//}*/
	//outColor = vec4(0, 0, uv.z , 1.0);
	//outColor = vec4(ShadowCoord);
	outColor = vec4(debugColor, 1.0);
}