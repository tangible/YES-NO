uniform sampler2D tex;
varying vec2 texture_coordinate; 

void main()
{
	gl_FragColor = texture2D(tex, (1.0-texture_coordinate)-vec2(0.35, 0.2));

}
