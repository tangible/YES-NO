/*
 *  LightScattering.cpp
 *  akiraOF42ofxMyShading
 *
 *  Created by Makira on 10/11/12.
 *  Copyright 2010 yesMAYBEno. All rights reserved.
 *
 */

#include "LightScattering.h"

void LightScattering::setup(float _exposure,
							float _decay,
							float _density,
							float _weight,
							ofxVec2f _lightPositionOnScreen,
							int _useBoundingShape,
							int _boudingShape) {

	colFBO.setup(ofGetWidth(), ofGetHeight());
	
	ofxMyShading::setDataPathRoot();
	shader.setup("LightScattering");
	ofxMyShading::resetDataPathRoot();
	
	exposure = _exposure;
	decay = _decay;
	density = _density;
	weight = _weight;
	lightPositionOnScreen = _lightPositionOnScreen;
	useBoundingShape = _useBoundingShape;
	boudingShape = _boudingShape;

}

void LightScattering::update(float _exposure,
							 float _decay,
							 float _density,
							 float _weight,
							 ofxVec2f _lightPositionOnScreen,
							 int _useBoundingShape,
							 int _boudingShape) {
	
	exposure = _exposure;
	decay = _decay;
	density = _density;
	weight = _weight;
	lightPositionOnScreen = _lightPositionOnScreen;
	useBoundingShape = _useBoundingShape;
	boudingShape = _boudingShape;
	
}

void LightScattering::beginFeedingTargetObjDuringYourUpdateCycle() {
	
	colFBO.beforeUpdate();
	
}

void LightScattering::endFeedingTargetObjDuringYourUpdateCycle() {
	
	colFBO.afterUpdate();
	
}

void LightScattering::drawScatteringObjInYourDrawCycleButBeforeCameraUpdate(int blankTexUnit, int _useBoundingShape, int _boudingShapeTexUnit) {
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	colFBO.beforeDraw(blankTexUnit);
	shader.begin();
	shader.setUniform1i("myTexture", blankTexUnit);
	shader.setUniform1f("exposure", exposure);
	shader.setUniform1f("decay", decay);
	shader.setUniform1f("density", density);
	shader.setUniform1f("weight", weight);
	shader.setUniform2f("lightPositionOnScreen", lightPositionOnScreen.x, lightPositionOnScreen.y);
	shader.setUniform1i("useBoundingShape", _useBoundingShape);
	shader.setUniform1i("boudingShape", _boudingShapeTexUnit);
	drawFullScreenQuad();
	shader.end();
	colFBO.afterDraw();	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void LightScattering::drawFullScreenQuad() {
	//glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);
	glTexCoord3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glTexCoord3f(1.0, 1.0, 0.0);
	glVertex3f(ofGetWidth(), 0, 0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(ofGetWidth(), ofGetHeight(), 0);
	glTexCoord3f(0.0, 0.0, 0.0);
	glVertex3f(0, ofGetHeight(), 0);
	glEnd();	
	ofSetColor(255, 255, 255);
	//glEnable(GL_DEPTH_TEST);
}