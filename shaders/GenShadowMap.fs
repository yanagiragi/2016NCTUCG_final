#version 330 core

// Ouput data
//layout(location = 0) out float fragmentdepth;
out vec4 outColor;

void main(){
    // Not really needed, OpenGL does it anyway
	float fragmentdepth = gl_FragCoord.z;//pow(gl_FragCoord.z, 10000);
	outColor = vec4(fragmentdepth, fragmentdepth, fragmentdepth, 1.0);
}