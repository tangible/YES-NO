/*
 *  MetaBallChunk.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/02.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "Metaballs.h"
#include "ofxVectorMath.h"
#include "ofxTween.h"
#include "HTTPSMSClient.h"
#include "ofxColorPicker.h"


class MetaBallChunk {
	
public:
	
	MetaBallChunk(int points, int _chunkID);
	void updateChunkBasePos();
	void updateBallSizes();
	void updateColor();
	void minimizeOne();
	
	// sms events
	void onSMSRecievedChangeCol(float thisTime, float total, ofColor col);
	void onSMSRecievedChangeMetaballSize(float thisTime, float total);	
	void updateForSMS(ofxVec3f centroid, ofPoint boundsAvg, float boundsScaling);
	void drawForSMS(ofxVec3f centroid, ofPoint boundsAvg, float boundsScaling);
	ofxTween ballPosTweenX;
	ofxTween ballPosTweenY;
	ofxTween ballPosTweenZ;
	
	
	int						nPoints;
	
	// fields for each metaballs
	CMetaballs*				m_pMetaballs;
	ofPoint*				ballPoints;
	ofPoint*				ballPointsPrev;
	ofPoint*				ballPointsPrev2;	
	float*					ballSizes;
	ofxTween				ballSizeTween;
	ofEvent<int>			onResizeComplete;
	int						minimizeTgt;
	
	// fields for this chunk
	int						chunkID;
	ofxVec3f				chunkCurrPos;
	ofxVec3f				chunkDestPos;
	float					deceleration;
	float					sizeBase;
	ofxVec4f				chunkCurrCol;
	ofxTween				chunkColTween;
	ofColor					chunkBaseCol;
	int						chunkColChangeTime;
	int						chunkColChanged;	
	float					chunkColChangeVal;

	
	// tweens
	ofxEasingBack		easingback;
	ofxEasingBounce 	easingbounce;
	ofxEasingCirc		easingcirc;
	ofxEasingCubic		easingcubic;
	ofxEasingElastic	easingelastic;
	ofxEasingExpo		easingexpo;
	ofxEasingLinear 	easinglinear;
	ofxEasingQuad		easingquad;
	ofxEasingQuart		easingquart;
	ofxEasingQuint		easingquint;
	ofxEasingSine		easingsine;
	
};