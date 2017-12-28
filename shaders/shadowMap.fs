#version 330 core

// Ouput data
//layout(location = 0) out float fragmentdepth;
in vec4 pos;
out vec4 outColor;

void main(){
    // Not really needed, OpenGL does it anyway
	// fragmentdepth = gl_FragCoord.z;

	//fragmentdepth = (pos.z/pos.w)*0.5+0.5;
	//fragmentdepth = gl_FragCoord.z;
	//	outColor = vec4((pos.z/pos.w)*0.5+0.5, (pos.z/pos.w)*0.5+0.5, (pos.z/pos.w)*0.5+0.5, 1.0);
	//	outColor = pos;
	outColor = vec4(1, 0, 0, 0);
}