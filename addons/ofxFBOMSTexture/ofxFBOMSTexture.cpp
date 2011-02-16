/*
 *  ofxFBOMSTexture.cpp
 *  openFrameworks
 *
 *  Created by Zach Gage on 3/28/08.
 *  Copyright 2008 STFJ.NET. All rights reserved.
 *
 *  http://addons.openframeworks.cc/projects/show/ofxfbotexture
 *
 *  Updated 22nd July 2009
 *
 *  Modified by James George (jamesgeorge.org)
 *  Multisampling added December 1st 2009
 *  Big Help provided by:
 *  http://blog.dexta.ch/2008/08/27/gl_ext_framebuffer_object-with-multisampling/
 */
#include "ofxFBOMSTexture.h"

void ofxFBOMSTexture::initFBOs()
{
	
	if(!GL_EXT_framebuffer_multisample){
		cout << "Warning: multisampled not supported on your hardware" << endl;
	}
		
	// Create the render buffer for depth
	glGenRenderbuffersEXT(1, &colorBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, colorBuffer);
	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, NUM_SAMPLES, GL_RGBA8, texData.tex_w, texData.tex_h);
	
	// Create the render buffer for depth
	glGenRenderbuffersEXT(1, &depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
	glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, NUM_SAMPLES, GL_DEPTH_COMPONENT, texData.tex_w, texData.tex_h);
	
	// create & setup FBO
	glGenFramebuffersEXT(1, &mfbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfbo);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, colorBuffer);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
	
	checkStatus();
	
	// create & setup texture
	glGenTextures(1, (GLuint *)&texData.textureID);   // could be more then one, but for now, just one
	glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
	glTexImage2D(texData.textureTarget, 0, texData.glType, texData.tex_w, texData.tex_h, 0, texData.glType, GL_UNSIGNED_BYTE, 0);
	
	//texture parameters
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// create & setup regular FBO
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	
	// attach it to the FBO so we can render to it
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, texData.textureTarget, (GLuint)texData.textureID, 0);
	
	checkStatus();
	
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}


void ofxFBOMSTexture::swapIn() {
    _isActive = true;
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mfbo);

	if(autoClear) clear();
	
}


void ofxFBOMSTexture::swapOut() {
	_isActive = false;

	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, mfbo);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, fbo);
	glBlitFramebufferEXT(0, 0, texData.tex_w, texData.tex_h, 0, 0, texData.tex_w, texData.tex_h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	// you have to unbind all fbos before you can render to the main window
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void ofxFBOMSTexture::bindMRTTexture(){
	glActiveTexture(GL_TEXTURE0);
	bindAsTexture();
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(texData.textureTarget, (GLuint)depthTextureID);	
}


void ofxFBOMSTexture::clean()
{
	// try to free up the texture memory so we don't reallocate
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
	if (texData.textureID != 0){
		glDeleteRenderbuffersEXT(1, &depthBuffer);
		glDeleteRenderbuffersEXT(1, &colorBuffer);
		glDeleteFramebuffersEXT(1, &fbo);
		glDeleteFramebuffersEXT(1, &mfbo);
		glDeleteTextures(1, (GLuint *)&texData.textureID);
//		if(mrtTexture1 != 0){
//			glDeleteTextures(1, (GLuint *)&mrtTexture1);
//		}
	}
	texData.width = 0;
	texData.height = 0;
	texData.bFlipTexture = false;
}
