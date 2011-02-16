#ifndef _MX_SHADER
#define _MX_SHADER

#include "ofMain.h"
#include "ofxShader.h" 
#include "ofxFBOTexture.h"

class mxShader{

	public:
		void setup(int w, int h);
		
		void beginRender();
		void endRender();
		
		void setParams();
		void draw(float x, float y, float w, float h);
				
		ofxShader _mxShader;
		ofxFBOTexture _mxBase;
		ofxFBOTexture _mxBlend;

};

#endif

// cheers / mark.cetilia.org