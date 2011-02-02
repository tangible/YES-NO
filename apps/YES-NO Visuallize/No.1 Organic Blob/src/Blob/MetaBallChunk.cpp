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
	chunkDestPos = ofxVec3f(10.0, 10.0, 0.0);
	deceleration = 0.99;
	sizeBase = 0.2;
	minimizeTgt = -1;
	nPoints = points;
	
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
	m_pMetaballs->SetGridSize(100);
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
		
	}

//	cout << "distance = " + ofToString(chunkCurrPos.distance(chunkDestPos)) << endl;
//	cout << "chunk curr x = " + ofToString(chunkCurrPos.x) << endl;
//	cout << "chunk curr y = " + ofToString(chunkCurrPos.y) << endl;
//	cout << "chunk dest x = " + ofToString(chunkDestPos.x) << endl;
//	cout << "chunk dest y = " + ofToString(chunkDestPos.y) << endl;
//	cout << " " << endl;
	
}

void MetaBallChunk::updateBallSizes() {

	for (int i = 0; i < nPoints; i++) {
		float sizeBaseSin  = 0.035 * sin(ofGetElapsedTimeMillis()/4000.0);
		float sizeLevelSin = 0.010 * sin(ofGetElapsedTimeMillis()/1300.0);
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
