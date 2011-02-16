#include "mxShader.h"

//--------------------------------------------------------------
void mxShader::setup(int w, int h){	
	_mxBase.allocate(w, h, GL_RGBA, 4);
	_mxBlend.allocate(w, h, GL_RGBA, 4);
	_mxShader.setup("mx");
}

//--------------------------------------------------------------
void mxShader::draw(float x, float y, float w, float h){
	
	_mxBase.begin();
	{
		glColor4f(1,0,0,1);
		ofCircle(412, 384, 150);
	}
	_mxBase.end();

	glActiveTexture(GL_TEXTURE3);
	glBindBuffer(GL_TEXTURE_RECTANGLE_ARB, (GLuint)_mxBase.texData.textureID);
//	glBindBuffer(GL_TEXTURE_2D, (GLuint)_mxBase.texData.textureID);	
	_mxBase.draw(0, 0, w, h);

	_mxBlend.begin();
	{
		glColor4f(0,1,0,1);
		ofCircle(612, 384, 150);
	}
	_mxBlend.end();

	glActiveTexture(GL_TEXTURE1);
	glBindBuffer(GL_TEXTURE_RECTANGLE_ARB, (GLuint)_mxBlend.texData.textureID);
//	glBindBuffer(GL_TEXTURE_2D, (GLuint)_mxBlend.texData.textureID);	
	_mxBlend.draw(0, 0, w, h);
		
	_mxShader.begin();
	{
		_mxShader.setUniform1i("BaseImage", 3);
		_mxShader.setUniform1i("BlendImage", 1);
		_mxShader.setUniform1i("Opacity", 1.); 

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(0, 0);
			
			glTexCoord2f(w, 0);
			glVertex2f(w, 0);
			
			glTexCoord2f(w, h);
			glVertex2f(w, h);
			
			glTexCoord2f(0, h);
			glVertex2f(0, h);
		}
		glEnd();
	}	
	_mxShader.end();
	
}
