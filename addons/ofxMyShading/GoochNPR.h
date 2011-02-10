/*
 *  GoochNPR.h
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofxMyShading.h"

class GoochNPR : public ofxMyShading {

public:
	
	void setup(ofxVec3f _LightPos = ofxVec3f(100.0, -500.0, -100.0),
			   ofxVec3f _WarmColour = ofxVec3f(0.8, 0.8, 0.8),
			   ofxVec3f _CoolColour = ofxVec3f(0.7, 0.7, 0.7),
			   ofxVec3f _SurfaceColour = ofxVec3f(0.0, 0.0, 0.0),
			   float _OutlineWidth = 0.5);
	
	void beginRender();
	void endRender();
	
	
private:
	
	// vert uniforms
	ofxVec3f LightPos;
	
	// frag uniforms
	ofxVec3f WarmColour;
	ofxVec3f CoolColour;
	ofxVec3f SurfaceColour;
	float OutlineWidth;
	

};