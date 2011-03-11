/*
 *  MetaBallChunk.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/02.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "MetaBallChunk.h"
int gridSize = 160;
float baseBallSize = 0.1;
float maxBallSize = 0.9;

float baseZ = 0;
MetaBallChunk::MetaBallChunk(int points, int _chunkID) {
	
	chunkCurrPos = ofxVec3f(0.0, 0.0, 0.0);
	if (_chunkID == 0) {
		chunkDestPos = ofxVec3f(-400.0, 0.0, baseZ);
	}else if (_chunkID == 1) {
		chunkDestPos = ofxVec3f(400.0, 0.0, baseZ);		
	}
	deceleration = ofRandom(0.98, 0.99);
	sizeBase = baseBallSize;
	minimizeTgt = -1;
	nPoints = points;
	
	if (_chunkID == 0) {
		chunkBaseCol.r = 0.0; chunkBaseCol.g = 0.5; chunkBaseCol.b = 0.0;
		chunkColTween.setParameters(easingcirc, ofxTween::easeInOut, 0.5, 0.5, 0, 0);
		chunkColTween.addValue(0.0, 0.0);
		chunkColTween.addValue(0.0, 0.0);
	}else {
		chunkBaseCol.r = 0.5; chunkBaseCol.g = 0.0; chunkBaseCol.b = 0.0;
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
		ballSizes[i] = baseBallSize;
	}
	m_pMetaballs = new CMetaballs(nPoints);
	m_pMetaballs->SetGridSize(gridSize);
	chunkID = _chunkID;	
	
	ballSizeTween.setParameters(easingback, ofxTween::easeOut,sizeBase,sizeBase,0,0);
	ballPosTweenX.setParameters(easingcirc, ofxTween::easeIn, 0, -ofGetScreenWidth()/2, 0, 0);
	ballPosTweenY.setParameters(easingcirc, ofxTween::easeIn, 0, -ofGetScreenHeight()/2, 0, 0);	
	ballPosTweenZ.setParameters(easingcirc, ofxTween::easeIn, 0, 0, 0, 0);		
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
		chunkDestPos = ofxVec3f((chunkID == 0) ? -400.0 : 400.0, 
								ofRandom(-50, 50), 
								ofRandom(-100, 100));								
//								baseZ);
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
		ballSizeTween.setParameters(1,easingquad,ofxTween::easeInOut,sizeNow,sizeBase,500,0);
	}
	
	for (int i = 0; i < nPoints; i++) {
		ballSizes[i] = sizeNow;// + sizeBaseSin;
	}
	
}

void MetaBallChunk::minimizeOne() {

	if (-1 == minimizeTgt) 
		minimizeTgt = ofRandom(0, nPoints);
	
	if (ballSizes[minimizeTgt] < -1.0) 
		minimizeTgt = -1;

}

void MetaBallChunk::onSMSRecievedChangeCol(int _yesORno, float thisTime, float total, ofColor col) {

	// color
	chunkColChangeVal = ofMap(thisTime, 0.0, 1.0, 0.0001, 1.0);
	if (chunkID == _yesORno) {
		chunkColChangeTime = ofMap(thisTime, 0.0, 1.0, 1.0, 4.0); 
	}else {
		chunkColChangeTime = 0;
	}
	chunkBaseCol = col;
	chunkColChanged = 0;

}

void MetaBallChunk::onSMSRecievedChangeMetaballSize(int _yesORno, float thisTime, float total) {
	int maxSMSnum = 5000;
	total = ofClamp(total, 0, maxSMSnum);
	float tmpSize = thisTime*total;
	float maxSizeParam = 1.0*maxSMSnum;
	sizeBase = ofMap(tmpSize, 0.0, maxSizeParam, baseBallSize, maxBallSize);
	
	float diffFactor = ofMap(thisTime, 0.0, 1.0, 0.5, 1.5);
	sizeBase *= diffFactor;
	
	cout << "thisTime = " + ofToString(thisTime) << endl;
	cout << "maxSizeParam = "+ofToString(maxSizeParam) << endl;
	string yn = (chunkID==0)?"YES":"NO";
	cout << "sizeBase "+yn+" = "+ofToString(sizeBase) << endl;
//	sizeBase = ofMap(thisTime, 0.0, 1.0, baseBallSize, maxBallSize);
	
	float mapForSize = ofMap(thisTime, 0.0, 1.0, sizeBase, sizeBase);
	if (chunkID == _yesORno) {
		mapForSize = ofMap(thisTime, 0.0, 1.0, sizeBase, sizeBase*2);
	}
		
	float mapForSizeDur = ofMap(thisTime, 0.0, 1.0, 100, 100);
	ballSizeTween.setParameters(easingbounce,ofxTween::easeInOut,
								sizeBase,mapForSize,
								mapForSizeDur,0);
	
}

void MetaBallChunk::updateForSMS(ofxVec3f centroid, ofPoint boundsAvg, float boundsScaling) {
	
	// base pos
	ofxVec3f curPos = ofxVec3f(ofGetScreenWidth()/2-centroid.x, ofGetScreenHeight()/2-centroid.y, centroid.z);
	curPos = -curPos;
	float degree = ofGetFrameNum()*10;
	float radian = (degree/180)*PI;
	chunkCurrPos.x = curPos.x+cos(radian)*300;
	chunkCurrPos.y = curPos.y+sin(radian)*300;
	
	// balls pos
	for (int i = 0; i < nPoints; i++) {
		float bx = (chunkCurrPos.x - boundsAvg.x) * boundsScaling;
		float by = (chunkCurrPos.y - boundsAvg.y) * boundsScaling;
		float bz = (chunkCurrPos.z - boundsAvg.z) * boundsScaling;

		ballPoints[i].set(bx, by, bz);		
		
	}

	updateBallSizes();
	m_pMetaballs->UpdateBallsFromPointsAndSizes(nPoints, ballPoints, ballSizes);
}

void MetaBallChunk::drawForSMS(ofxVec3f centroid, ofPoint boundsAvg, float boundsScaling) {

	ofxVec3f curPos = ofxVec3f(ofGetScreenWidth()/2-centroid.x, ofGetScreenHeight()/2-centroid.y, centroid.z);
	chunkCurrPos = -curPos;	
	
	ofxVec4f basecol = chunkCurrCol;
	glColor3f(basecol.x, basecol.y, basecol.z);				
	
	m_pMetaballs->Render();

}