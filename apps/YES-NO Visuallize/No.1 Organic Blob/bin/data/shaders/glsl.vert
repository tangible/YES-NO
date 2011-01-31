varying vec2 texture_coordinate; 
varying vec3 light_position;
varying vec3 light_half_vector;
varying vec3 normal;

vec2 genSphere(in vec3 ecPosition3, in vec3 normal) {

   float m;
   vec3 r, u;
   u = normalize(ecPosition3);
   r = reflect(u, normal);
   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0));
   return vec2(r.x / m + 0.5, r.y / m + 0.5);
   
}

void main() {

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;

	vec4 ecPosition  = gl_ModelViewMatrix * gl_Vertex;
	vec3 ecPosition3 = (vec3(ecPosition)) / ecPosition.w;
	
	normal = gl_NormalMatrix * gl_Normal;
	normal = normalize(normal);	
	texture_coordinate = genSphere(ecPosition3, normal).xy;
	
	light_position = normalize(gl_LightSource[0].position.xyz);
	light_half_vector = normalize(gl_LightSource[0].halfVector.xyz);
}
