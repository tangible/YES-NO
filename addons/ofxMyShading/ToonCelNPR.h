/*
 *  ToonCelNPR.h
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofxMyShading.h"

class ToonCelNPR : public ofxMyShading {

public:
	
	void setup(ofxVec3f _LightPos = ofxVec3f(100.0, -500.0, -100.0),
			   ofxVec4f _HighlightColour = ofxVec4f(0.9, 0.9, 0.9, 1.0),
			   ofxVec4f _MidColour = ofxVec4f(0.7, 0.7, 0.7, 1.0),
			   ofxVec4f _ShadowColour = ofxVec4f(0.5, 0.5, 0.5, 0.5),
			   float _HighlightSize = 0.4,
			   float _ShadowSize = 0.001,
			   float _OutlineWidth = 0.6,
			   ofxVec4f _OutlineColor = ofxVec4f(0.3, 0.3, 0.3, 1.0));
	
	void beginRender();
	void endRender();
	

private:
	
	// vert uniforms
	ofxVec3f LightPos;

	// frag uniforms
	ofxVec4f HighlightColour;
	ofxVec4f MidColour;
	ofxVec4f ShadowColour;
	float HighlightSize;
	float ShadowSize;
	float OutlineWidth;
	ofxVec4f OutlineColor;
	
};