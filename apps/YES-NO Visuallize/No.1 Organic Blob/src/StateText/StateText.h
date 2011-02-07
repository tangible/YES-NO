/*
 *  StateText.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/06.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxTween.h"
#include "HTTPClient.h"
#include "ofxVectorMath.h"
#include "ofx3DUtils.h"

class StateText {

public:
	
	void setup();
	void onSMSReceivedUpdate(int yesORno, UpdateInfo upInfo);
	void draw(UpdateInfo upInfo, ofxVec3f centroidYes, ofxVec3f centroidNo);
	
	ofTrueTypeFont font;	
	ofxTween yesXTween;
	ofxTween yesYTween;
	ofxTween noXTween;
	ofxTween noYTween;
	ofxTween yesSizeTween;
	ofxTween noSizeTween;
	ofxEasingCirc easingcirc;
	
	float minSize;
	float maxSize;	
	
	string yes;
	string no;
	string yesNum;
	string noNum;
	float yesSize;
	float noSize;
};