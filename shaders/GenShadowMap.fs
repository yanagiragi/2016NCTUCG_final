#version 330 core

// Ouput data
//layout(location = 0) out float fragmentdepth;
out vec4 outColor;

void main(){
    // Not really needed, OpenGL does it anyway
	//fragmentdepth = 0.1;
	float fragmentdepth = pow(gl_FragCoord.z, 10000);
	outColor = vec4(fragmentdepth, fragmentdepth, fragmentdepth, 1.0);
}