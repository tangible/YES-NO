/*
 *  ofxDepthFBO.cpp
 *  try_DepthBuffer
 *
 *  Created by Makira on 10/11/08.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#include "ofxDepthFBO.h"

void ofxDepthFBO::setup(int width, int height) {
	
//	depthTex.allocate(width, height, GL_DEPTH_COMPONENT);
//	tex = depthTex.texData.textureID;
	
	tex = depthTex.genDepthTexture(width, height);

	glGenFramebuffersEXT(1, &fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
							  GL_TEXTURE_2D, tex, 0);

	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);

}

void ofxDepthFBO::beforeUpdate() {

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

}

void ofxDepthFBO::afterUpdate() {

	//glDisable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

}

void ofxDepthFBO::beforeDraw(int texUnit) {
	
	glActiveTexture(GL_TEXTURE0+texUnit);
//	glActiveTextureARB(GL_TEXTURE0+texUnit);	
	glBindTexture(GL_TEXTURE_2D, tex);

}

void ofxDepthFBO::afterDraw() {

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
//	glActiveTextureARB(GL_TEXTURE0);	
	
}