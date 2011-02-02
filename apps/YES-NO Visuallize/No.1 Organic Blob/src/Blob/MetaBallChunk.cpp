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
	chunkDestPos = ofxVec3f(ofRandom(-300, 300), ofRandom(-300, 300), 0.0);
	deceleration = ofRandom(0.989, 0.9999);
	sizeBase = ofRandom(0.15, 0.15);
	minimizeTgt = -1;
	nPoints = points;
	
	chunkCurrCol = ofxVec4f(ofRandomuf(), ofRandomuf(), ofRandomuf(), 1.0);
	chunkDestCol = ofxVec4f(ofRandomuf(), ofRandomuf(), ofRandomuf(), 1.0);
	decelerationCol = ofRandom(0.989, 0.9999);
	whichCol = ofRandom(1, 3);
	
	ballPoints     = new ofPoint[nPoints];
	ballPointsPrev = new ofPoint[nPoints];
	ballPointsPrev2= new ofPoint[nPoints];
	ballSizes     = new float[nPoints];
	for (int i=0; i < nPoints; i++){
		ballPoints[i].set(0.0,0.0,0.0);
		ballPointsPrev[i].set(0.0,0.0,0.0);
		ballPointsPrev2[i].set(0.0,0.0,0.0);
		ballSizes[i] = 1.0;
	}
	m_pMetaballs = new CMetaballs(nPoints);
	m_pMetaballs->SetGridSize(120);
	chunkID = _chunkID;	
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
		chunkDestPos = ofxVec3f(ofRandom(-300, 300), 
								ofRandom(-300, 300), 
								ofRandom(-300, 300));
		deceleration = ofRandom(0.989, 0.9999);
		
	}

//	cout << "distance = " + ofToString(chunkCurrPos.distance(chunkDestPos)) << endl;
//	cout << "chunk curr x = " + ofToString(chunkCurrPos.x) << endl;
//	cout << "chunk curr y = " + ofToString(chunkCurrPos.y) << endl;
//	cout << "chunk dest x = " + ofToString(chunkDestPos.x) << endl;
//	cout << "chunk dest y = " + ofToString(chunkDestPos.y) << endl;
//	cout << " " << endl;
	
}

void MetaBallChunk::updateColor() {
	
	if (0.1 < chunkCurrCol.distance(chunkDestCol)) {
//		for (int i = 0; i < whichCol; i++) {
//			float amp = chunkDestCol[i] - chunkCurrCol[i];
//			float tmp = amp * decelerationCol;
//			chunkCurrCol[i] += amp - tmp;
//		}
		
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
	
//	cout << "distance = " + ofToString(chunkCurrCol.distance(chunkDestCol)) << endl;
//	cout << "chunk curr x = " + ofToString(chunkCurrCol.x) << endl;
//	cout << "chunk curr y = " + ofToString(chunkCurrCol.y) << endl;
//	cout << "chunk dest x = " + ofToString(chunkCurrCol.x) << endl;
//	cout << "chunk dest y = " + ofToString(chunkCurrCol.y) << endl;
//	cout << " " << endl;	
}

void MetaBallChunk::updateBallSizes() {

	for (int i = 0; i < nPoints; i++) {
//		float sizeBaseSin  = 0.035 * sin(ofGetElapsedTimeMillis()/4000.0);
//		float sizeLevelSin = 0.010 * sin(ofGetElapsedTimeMillis()/1300.0);
		float sizeBaseSin  = 0.035 * ofNoise(ofGetElapsedTimeMillis());
		float sizeLevelSin = 0.010 * ofNoise(ofGetElapsedTimeMillis());
		ballSizes[i] = sizeBase + sizeBaseSin + sizeLevelSin;	
	}
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
