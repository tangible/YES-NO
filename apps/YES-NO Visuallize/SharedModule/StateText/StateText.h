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
#include "HTTPSMSClient.h"
#include "ofxVectorMath.h"
#include "ofx3DUtils.h"
#include "ofxColorPicker.h"

class StateText {

public:
	
	void setup();
	void onSMSReceivedUpdate(int yesORno, UpdateInfo upInfo);
	void draw(UpdateInfo upInfo, ofxVec3f centroidYes, ofxVec3f centroidNo);
	void draw(UpdateInfo upInfo);
	void drawWithNoScale(UpdateInfo upInfo, ofColor yesCol, ofColor noCol);	
	void drawWithNoScale(UpdateInfo upInfo, float colAngYes, float colAngNo);
	void drawWithNoScale(UpdateInfo upInfo);
	void updateColor(ofColor yesCol, ofColor noCol);
	void updateColor(float colAngYes = 1000.0, float colAngNo = 1000.0);
	void updateColorYes(float colAngYes = 1000.0);
	void updateColorNo(float colAngNo = 1000.0);
	void updateColorYes(ofColor _yesCol);
	void updateColorNo(ofColor _noCol);
	void startFadeToDefaultColorYes(float dur = 2000);
	void startFadeToDefaultColorNo(float dur = 2000);
	string insertComma(string num);
	
	ofTrueTypeFont dfont;
	
	ofTrueTypeFont numberFont;
	static const int numberFontSize = 100;
	ofTrueTypeFont answerFont;
	static const int answerFontSize = 30;
	
	ofxColorPicker colp;
	ofColor	 curYesCol;
	ofColor  curNoCol;
	ofxTween yesColTween;
	ofxTween noColTween;
	ofxEasingCirc colTweenEasing;
	
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