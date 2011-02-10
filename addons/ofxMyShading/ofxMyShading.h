/*
 *  ofxMyShading.h
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxShader.h"

class ofxMyShading {

protected:
	
	void setDataPathRoot();
	void resetDataPathRoot();
	
	ofxShader shader;

};