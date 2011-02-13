/*
 *  AnisotropicSpecularReflection.h
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofxMyShading.h"

class AnisotropicSpecularReflection : public ofxMyShading {

public:
	
	void setup(ofxVec3f _LightPos = ofxVec3f(100.0, 1500.0, -100.0),
			   ofxVec3f _AmbientColour = ofxVec3f(0.5, 0.5, 0.5),
			   ofxVec3f _DiffuseColour = ofxVec3f(0.5, 0.5, 0.5),
			   ofxVec3f _SpecularColour = ofxVec3f(1.0, 1.0, 1.0),
			   float _AmbientIntensity = 0.1,
			   float _DiffuseIntensity = 0.1,
			   float _SpecularIntensity = 2.5,
			   float _Roughness = 0.2,
			   float _AnisoRoughness = 0.5,
			   ofxVec3f _SpecDirection = ofxVec3f(0.0, 100.0, 100.0));
	
	void beginRender();
	void endRender();
	
private:
	
	// vert uniforms
	ofxVec3f LightPos;
	
	// frag uniforms;
	ofxVec3f AmbientColour;
	ofxVec3f DiffuseColour;
	ofxVec3f SpecularColour;
	float AmbientIntensity;
	float DiffuseIntensity;
	float SpecularIntensity;
	float Roughness;
	float AnisoRoughness;
	ofxVec3f SpecDirection;	
	
};