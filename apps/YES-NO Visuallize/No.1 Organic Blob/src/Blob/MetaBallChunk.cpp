/*
 *  MetaBallChunk.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/02.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "MetaBallChunk.h"

MetaBallChunk::MetaBallChunk(int points, int _chunkID) {
	
	chunkCurrPos = ofxVec3f(0.0, 0.0, 0.0);
	chunkDestPos = ofxVec3f(ofRandom(-400, 400), ofRandom(-400, 400), 0.0);
	deceleration = ofRandom(0.989, 0.9999);
	sizeBase = ofRandom(0.2, 0.2);
	minimizeTgt = -1;
	nPoints = points;
	
	chunkCurrCol = ofxVec4f(ofRandomuf(), ofRandomuf(), ofRandomuf(), 1.0);
	chunkDestCol = ofxVec4f(ofRandomuf(), ofRandomuf(), ofRandomuf(), 1.0);
	decelerationCol = ofRandom(0.989, 0.9999);
	whichCol = ofRandom(1, 3);
	
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
	m_pMetaballs->SetGridSize(120);
	chunkID = _chunkID;	
	
	unsigned delayForever = 100000000;
	ballSizeTween.setParameters(1,easingback,ofxTween::easeOut,sizeBase,sizeBase*3,1000,delayForever);
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
		chunkDestPos = ofxVec3f(ofRandom(-500, 500), 
								ofRandom(-500, 500), 
								ofRandom(-500, 500));
		deceleration = ofRandom(0.989, 0.9999);
		
	}

}

void MetaBallChunk::updateColor() {
	
	if (0.1 < chunkCurrCol.distance(chunkDestCol)) {
		float ampx = chunkDestCol.x - chunkCurrCol.x;
		float ampy = chunkDestCol.y - chunkCurrCol.y;
		float ampz = chunkDestCol.z - chunkCurrCol.z;
		float tmpampx = ampx * decelerationCol;
		float tmpampy = ampy * decelerationCol;
		float tmpampz = ampz * decelerationCol;
		chunkCurrCol.x += ampx - tmpampx;
		chunkCurrCol.y += ampy - tmpampy;
		chunkCurrCol.z += ampz - tmpampz;		
	}else {
		chunkDestCol = ofxVec4f(ofRandomuf(), ofRandomuf(), ofRandomuf(), 1.0);
		decelerationCol = ofRandom(0.9, 0.95);
	}

}

void MetaBallChunk::updateBallSizes() {
	
	float sizeNow = ballSizeTween.update();
	
	if (ballSizeTween.isCompleted())
		ballSizeTween.setParameters(1,easingquad,ofxTween::easeOut,sizeNow,sizeBase,500,0);
	
	for (int i = 0; i < nPoints; i++) {
		float sizeBaseSin  = 0.035 * ofNoise(ofGetElapsedTimeMillis());	
		ballSizes[i] = sizeNow + sizeBaseSin;
	}
	if (ballSizeTween.isRunning()) cout << "tween running: " + ofToString(ballSizeTween.update()) << endl;
}

void MetaBallChunk::minimizeOne() {

	if (-1 == minimizeTgt) 
		minimizeTgt = ofRandom(0, nPoints);
	
	if (ballSizes[minimizeTgt] < -1.0) 
		minimizeTgt = -1;
	
	//ballSizes[minimizeTgt] -= 0.1;

	cout << "minimizeTgt = " + ofToString(minimizeTgt) << endl;
	cout << "ballSizes[minimizeTgt] = " + ofToString(ballSizes[minimizeTgt]) << endl;
}

void MetaBallChunk::onSMSRecieved(float thisTime, float total) {
	
	// size
	sizeBase = ofMap(total, 0.0, 1.0, 0.05, 0.3);
	float mapForSize = ofMap(thisTime, 0.0, 1.0, sizeBase, sizeBase*3);
	float mapForSizeDur = ofMap(thisTime, 0.0, 1.0, 200, 1000);
	ballSizeTween.setParameters(easingelastic,ofxTween::easeOut,
								sizeBase,mapForSize,
								mapForSizeDur,0);
}
