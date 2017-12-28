uniform vec2 resolution;

uniform sampler2D tex;

void main()
{
    vec2 pos = gl_FragCoord.xy/resolution;

	vec4 col = texture2D(tex, pos);

	// Rescale by number of samples
	col /= col.w;

    gl_FragColor = vec4(col);
}