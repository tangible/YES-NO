// showdepth.frag
 
uniform sampler2D depth;
 
void main(void)
{
  gl_FragColor = texture2D(depth, gl_TexCoord[0].xy);
//  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}