uniform vec2 resolution;

uniform sampler2D tex;

void main()
{
    vec2 pos = gl_FragCoord.xy/resolution;

	vec4 col = texture2D(tex, pos);

	float depthValue = texture(tex, pos).z;
    vec4 FragColor = vec4(vec3(depthValue), 1.0);

	gl_FragColor = col;//vec4(depthValue, 0.0, 0.0, 1.0);
	gl_FragColor = vec4(depthValue, 0.0, 0.0, 1.0);
}