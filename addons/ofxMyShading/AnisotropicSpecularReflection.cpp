/*
 *  AnisotropicSpecularReflection.cpp
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/11.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#include "AnisotropicSpecularReflection.h"

void AnisotropicSpecularReflection::setup(ofxVec3f _LightPos,
										  ofxVec3f _AmbientColour,
										  ofxVec3f _DiffuseColour,
										  ofxVec3f _SpecularColour,
										  float _AmbientIntensity,
										  float _DiffuseIntensity,
										  float _SpecularIntensity,
										  float _Roughness,
										  float _AnisoRoughness,
										  ofxVec3f _SpecDirection) {

	ofxMyShading::setDataPathRoot();
	shader.setup("fluxus/aniso");
	ofxMyShading::resetDataPathRoot();
	
	LightPos = _LightPos;
	AmbientColour = _AmbientColour;
	DiffuseColour = _DiffuseColour;
	SpecularColour = _SpecularColour;
	AmbientIntensity = _AmbientIntensity;
	DiffuseIntensity = _DiffuseIntensity;
	SpecularIntensity = _SpecularIntensity;
	Roughness = _Roughness;
	AnisoRoughness = _AnisoRoughness;
	SpecDirection = _SpecDirection;

}

void AnisotropicSpecularReflection::beginRender() {
	
	shader.begin();
	shader.setUniform3f("AmbientColour", AmbientColour.x, AmbientColour.y, AmbientColour.z);
	shader.setUniform3f("DiffuseColour", DiffuseColour.x, DiffuseColour.y, DiffuseColour.z);
	shader.setUniform3f("SpecularColour", SpecularColour.x, SpecularColour.y, SpecularColour.z);
	shader.setUniform1f("AmbientIntensity", AmbientIntensity);
	shader.setUniform1f("DiffuseIntensity", DiffuseIntensity);
	shader.setUniform1f("SpecularIntensity", SpecularIntensity);
	shader.setUniform1f("Roughness", Roughness);
	shader.setUniform1f("AnisoRoughness", AnisoRoughness);
	shader.setUniform3f("SpecDirection", SpecDirection.x, SpecDirection.y, SpecDirection.z);
	shader.setUniform3f("LightPos", LightPos.x, LightPos.y, LightPos.z);
	
}

void AnisotropicSpecularReflection::endRender() {

	shader.end();

}