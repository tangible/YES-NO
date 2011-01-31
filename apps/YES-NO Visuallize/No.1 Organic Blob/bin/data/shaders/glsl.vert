varying vec2 texture_coordinate; 


vec2 genSphere(in vec3 ecPosition3, in vec3 normal)
{
   float m;
   vec3 r, u;
   u = normalize(ecPosition3);
   r = reflect(u, normal);
   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0));
   return vec2(r.x / m + 0.5, r.y / m + 0.5);
}



void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	vec4 ecPosition  = gl_ModelViewMatrix * gl_Vertex;
	vec3 ecPosition3 = (vec3(ecPosition)) / ecPosition.w;
	vec3 normal;
	normal = gl_NormalMatrix * gl_Normal;
	normal = normalize(normal);	
	texture_coordinate = genSphere(ecPosition3, normal).xy;
	
}
