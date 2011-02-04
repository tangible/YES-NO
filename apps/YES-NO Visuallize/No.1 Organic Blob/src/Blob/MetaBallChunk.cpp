/*
 *  MetaBallChunk.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/02.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "MetaBallChunk.h"
int gridSize = 110;
float baseBallSize = 0.10;
float maxBallSize = 0.60;
MetaBallChunk::MetaBallChunk(int points, int _chunkID) {
	
	chunkCurrPos = ofxVec3f(0.0, 0.0, 0.0);
	if (_chunkID == 0) {
		chunkDestPos = ofxVec3f(400.0, 0.0, 0.0);
	}else {
		chunkDestPos = ofxVec3f(-400.0, 0.0, 0.0);		
	}
	deceleration = ofRandom(0.98, 0.99);
	sizeBase = baseBallSize;
	minimizeTgt = -1;
	nPoints = points;
	
	if (_chunkID == 0) {
		chunkBaseCol.r = 0.5; chunkBaseCol.g = 0.0; chunkBaseCol.b = 0.0;
		chunkColTween.setParameters(easingcirc, ofxTween::easeInOut, 0.5, 0.5, 0, 0);
		chunkColTween.addValue(0.0, 0.0);
		chunkColTween.addValue(0.0, 0.0);
	}else {
		chunkBaseCol.r = 0.0; chunkBaseCol.g = 0.5; chunkBaseCol.b = 0.0;
		chunkColTween.setParameters(easingcirc, ofxTween::easeInOut, 0.0, 0.0, 0, 0);
		chunkColTween.addValue(0.5, 0.5);
		chunkColTween.addValue(0.0, 0.0);		
	}
	chunkColChangeTime = 0;
	
	
	ballPoints = new ofPoint[nPoints];
	ballPointsPrev = new ofPoint[nPoints];
	ballPointsPrev2 = new ofPoint[nPoints];
	ballSizes = new float[nPoints];
	for (int i=0; i < nPoints; i++){
		ballPoints[i].set(0.0,0.0,0.0);
		ballPointsPrev[i].set(0.0,0.0,0.0);
		ballPointsPrev2[i].set(0.0,0.0,0.0);
		ballSizes[i] = 1.0;
	}
	m_pMetaballs = new CMetaballs(nPoints);
	m_pMetaballs->SetGridSize(gridSize);
	chunkID = _chunkID;	
	
	ballSizeTween.setParameters(1,easingback,ofxTween::easeOut,sizeBase,sizeBase,0,0);
}

void MetaBallChunk::updateChunkBasePos() {
	
	if (1.0 < chunkCurrPos.distance(chunkDestPos)) {
		float ampx = chunkDestPos.x - chunkCurrPos.x;
		float ampy = chunkDestPos.y - chunkCurrPos.y;
		float ampz = chunkDestPos.z - chunkCurrPos.z;
		float tmpampx = ampx * deceleration;
		float tmpampy = ampy * deceleration;
		float tmpampz = ampz * deceleration;
		chunkCurrPos.x += ampx - tmpampx;
		chunkCurrPos.y += ampy - tmpampy;
		chunkCurrPos.z += ampz - tmpampz;
	}else {
		chunkDestPos = ofxVec3f(ofRandom(-700, 700), 
								ofRandom(-350, 350), 
								ofRandom(-100, 200));
		deceleration = ofRandom(0.97, 0.99);
		
	}

}

void MetaBallChunk::updateColor() {
	
	float r = chunkColTween.getTarget(0);
	float g = chunkColTween.getTarget(1);
	float b = chunkColTween.getTarget(2);
	
	// react to sms
	if (chunkColChangeTime != 0) {
		
		if (chunkColTween.isCompleted()) {
			chunkColTween.setParameters(easinglinear, ofxTween::easeInOut, r, chunkBaseCol.r+chunkColChangeVal/chunkColChangeVal, 100*chunkColChangeTime, 0);
			chunkColTween.addValue(g, chunkBaseCol.g+chunkColChangeVal/chunkColChangeVal);
			chunkColTween.addValue(b, chunkBaseCol.b+chunkColChangeVal/chunkColChangeVal);	
			chunkColChangeTime--;			
		}
		
	}else {
		if (chunkColTween.isCompleted()) {
			chunkColTween.setParameters(easingcirc, ofxTween::easeInOut, r, chunkBaseCol.r, 500, 0);
			chunkColTween.addValue(g, chunkBaseCol.g);
			chunkColTween.addValue(b, chunkBaseCol.b);
		}
	}
	
	chunkCurrCol.x = chunkColTween.update();
	chunkCurrCol.y = chunkColTween.getTarget(1);
	chunkCurrCol.z = chunkColTween.getTarget(2);	
}

void MetaBallChunk::updateBallSizes() {
	
	float sizeNow = ballSizeTween.update();
	
	if (ballSizeTween.isCompleted()) {
		//ofNotifyEvent(onResizeComplete, chunkID);
		ballSizeTween.setParameters(1,easingquad,ofxTween::easeInOut,sizeNow,sizeBase,500,0);
	}
	
	for (int i = 0; i < nPoints; i++) {
		//float sizeBaseSin  = 0.015 * ofNoise(ofGetElapsedTimeMillis());	
		ballSizes[i] = sizeNow;// + sizeBaseSin;
	}
	
}

void MetaBallChunk::minimizeOne() {

	if (-1 == minimizeTgt) 
		minimizeTgt = ofRandom(0, nPoints);
	
	if (ballSizes[minimizeTgt] < -1.0) 
		minimizeTgt = -1;

}

void MetaBallChunk::onSMSRecieved(float thisTime, float total) {
	
	// size
	sizeBase = ofMap(total, 0.0, 1.0, baseBallSize, maxBallSize);
	float mapForSize = ofMap(thisTime, 0.0, 1.0, sizeBase, sizeBase*2);
	float mapForSizeDur = ofMap(thisTime, 0.0, 1.0, 200, 1000);
	ballSizeTween.setParameters(easingelastic,ofxTween::easeInOut,
								sizeBase,mapForSize,
								mapForSizeDur,0);
	
	// color
	chunkColChangeVal = ofMap(thisTime, 0.0, 1.0, 0.0001, 1.0);
	chunkColChangeTime = ofMap(thisTime, 0.0, 1.0, 1.0, 4.0); 
	chunkColChanged = 0;

}
