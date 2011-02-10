// showdepth.vert
 
void main(void)
{
  gl_TexCoord[0] = gl_Vertex;
  gl_Position = vec4(gl_Vertex.xy * 2.0 - 1.0, 0.0, 1.0);
}