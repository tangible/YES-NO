uniform float exposure;
uniform float decay;
uniform float density;
uniform float weight;
uniform vec2 lightPositionOnScreen;
uniform sampler2D myTexture;
uniform bool useBoundingShape; //zero for false
uniform sampler2D boudingShape;

const int NUM_SAMPLES = 100 ;

void main() {
	
	vec2 textCoo = gl_TexCoord[0].st;
	
	if (useBoundingShape) {
		if (texture2D(boudingShape, textCoo ).r  == 0.0) {
			discard;
		}
	}

	vec2 deltaTextCoord = vec2( gl_TexCoord[0].st - lightPositionOnScreen.xy );
	deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
	float illuminationDecay = 1.0;
	
	for(int i=0; i < NUM_SAMPLES ; i++) {
			textCoo -= deltaTextCoord;
			vec4 sample = texture2D(myTexture, textCoo );
			sample *= illuminationDecay * weight;
			gl_FragColor += sample;
			illuminationDecay *= decay;
	}
	
	gl_FragColor *= exposure;
	
	float r = gl_FragColor.r;
	float g = gl_FragColor.g;
	float b = gl_FragColor.b;
	if (0.1 > r && 0.1 > g && 0.1 > b) 
		discard;

}
