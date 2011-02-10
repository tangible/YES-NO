/*
 *  BadPrintNPR.h
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofxMyShading.h"

class BadPrintNPR : public ofxMyShading {

public:
	
	void setup(ofxVec3f _LightPos = ofxVec3f(100.0, -500.0, -100.0),
			   ofxVec3f _Scale = ofxVec3f(20.0, 40.0, 0.0),
			   ofxVec3f _Offset = ofxVec3f(2.0, 2.0, 0.0),
			   ofxVec3f _Register = ofxVec3f(2.0, 2.0, 0.0),
			   ofxVec3f _Size = ofxVec3f(20.0, 20.0, 0.0));
	
	void beginRender();
	void endRender();
			
	
private:
	
	// vert uniforms
	ofxVec3f LightPos;
	
	// frag uniforms
	ofxVec3f Scale;
	ofxVec3f Offset;
	ofxVec3f Register;
	ofxVec3f Size;

};