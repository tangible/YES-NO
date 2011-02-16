/*
 *  ofxColorFBO.cpp
 *  try_DepthBuffer2
 *
 *  Created by Makira on 10/11/08.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#include "ofxColorFBO.h"

void ofxColorFBO::setup(int width, int height) {
	
	tex = colTex.genColorTexture(width, height);

	glGenFramebuffersEXT(1, &fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
							  GL_TEXTURE_2D, tex, 0);	
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
}

void ofxColorFBO::beforeUpdate() {
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	
}

void ofxColorFBO::afterUpdate() {
	
	glDisable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
}

void ofxColorFBO::beforeDraw(int texUnit) {
	
	glActiveTexture(GL_TEXTURE0+texUnit);
	glBindTexture(GL_TEXTURE_2D, tex);
	
}

void ofxColorFBO::afterDraw() {
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	
}