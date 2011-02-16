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

#ifndef _FBOMS_TEX
#define _FBOMS_TEX


#include "ofMain.h"
#include "ofxFBOTexture.h"

#include <iostream>

//change this depending on your graphics hardware
//determine how many samples you can use:
//int maxsamples;
//glGetIntegerv(GL_MAX_SAMPLES_EXT,&maxsamples);
#define NUM_SAMPLES 4


class ofxFBOMSTexture : public ofxFBOTexture {
  public:
	void swapIn();
	void swapOut();
	
	void bindMRTTexture();
	GLuint depthTextureID;
	
  protected:
	void initFBOs();
	void clean();
	
	//extra handles necessary for MultiSampling
	GLuint		colorBuffer;
	GLuint		colorBuffer1;
	GLuint		mfbo;
};

#endif
