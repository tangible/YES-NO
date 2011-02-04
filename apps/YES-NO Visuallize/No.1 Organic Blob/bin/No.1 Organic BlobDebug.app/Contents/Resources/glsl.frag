uniform sampler2D tex;
uniform float tex_col_mixRatio;
uniform float blob_transparency;
varying vec2 texture_coordinate; 
varying vec3 light_position;
varying vec3 light_half_vector;
varying vec3 normal;

void main() {

	vec4 rgbTex = texture2D(tex, vec2(texture_coordinate.x, 1.0-texture_coordinate.y)*0.5);
	
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec4 specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(max(dot(normal, light_half_vector), 0.0), gl_FrontMaterial.shininess);	
	float diffuse_value = max(dot(normal, light_position), 0.0);
	vec4 rgbGL = ambient + diffuse * diffuse_value + specular;
	
	vec3 mixed = mix(rgbGL.rgb, rgbTex.rgb, tex_col_mixRatio);
	gl_FragColor = vec4(mixed.r, mixed.g, mixed.b, blob_transparency);
	
//	gl_FragColor = rgbGL;
	
	
}
