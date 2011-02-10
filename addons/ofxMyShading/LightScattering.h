/*
 *  LightScattering.h
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/12.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofxMyShading.h"
#include "ofxColorFBO.h"

class LightScattering : public ofxMyShading {

public:
	
	void setup(float _exposure = 0.9,
			   float _decay = 0.9,
			   float _density = 0.9,
			   float _weight = 0.8,
			   ofxVec2f _lightPositionOnScreen = ofxVec2f(0.528, 0.425),
			   int _useBoundingShape = 0,
			   int _boudingShape = 0);
	void update(float _exposure = 0.9,
				float _decay = 0.9,
				float _density = 0.9,
				float _weight = 0.8,
				ofxVec2f _lightPositionOnScreen = ofxVec2f(0.528, 0.425),
				int _useBoundingShape = 0,
				int _boudingShape = 0);
	
	void beginFeedingTargetObjDuringYourUpdateCycle();
	void endFeedingTargetObjDuringYourUpdateCycle();
	void drawScatteringObjInYourDrawCycleButBeforeCameraUpdate(int blankTexUnit, int _useBoundingShape = 0, int _boudingShapeTexUnit = 0);

private:
	
	void drawFullScreenQuad();
	
	ofxColorFBO colFBO;
	
	float exposure;
	float decay;
	float density;
	float weight;
	ofxVec2f lightPositionOnScreen;
	int myTexture;
	int useBoundingShape;
	int boudingShape;

};