/*
 *  ConvexHull.h
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/09.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "YesNoObjectSoft.h"
#include "AdminPanel.h"
#include "IncomingSMS.h"
#include "HTTPSMSClient.h"

const int YES = 0;
const int NO = 1;

class ConvexHull {

public:
	
	void setup(int _fps, AdminPanel* _adminPanel, ofxCamera* _cam);
	void update();
	void draw();
	float feedSMS(UpdateInfo upInfo);
	void setupGLStuff();
	
	ofColor getYesSpikeColor() { return yesSoft.currentSpikeColor; }
	ofColor getNoSpikeColor() { return noSoft.currentSpikeColor; }
	float getYesSpikeColorFloat() { return yesSoft.currentSpikeColorFloat; }
	float getNoSpikeColorFloat() { return noSoft.currentSpikeColorFloat; }
	
	void scaleYes(int & z);
	void scaleNo(int & z);
	
	int						fps;
	ofxBullet*				bullet;
	ofxCamera*				cam;	
	AdminPanel*				admin;
	
	YesNoObjectSoft			yesSoft;
	YesNoObjectSoft			noSoft;		
	bool					isYesUpdating;
	bool					isNoUpdating;
	void					onFinishAllUpdating(int & yesOrNo);
	UpdateInfo				updateInfo;
	
	ofxVec3f				yesPoint;
	int						currentYesLevel;
	ofxVec3f				noPoint;
	int						currentNoLevel;
	
	vector<IncomingSMS*>	insmsYes;
	vector<IncomingSMS*>	insmsNo;
	vector<int>				yesFaceIdx;
	vector<int>				noFaceIdx;
	void onSmsReached(SmsInfo& smsInfo);
	void onSmsCompleted(SmsInfo& smsInfo);
	
	ofxTween yesScaleTween;
	ofxTween noScaleTween;
	ofxEasingElastic scaleEasing;
	float currYesScale;
	float currNOScale;
	static const float minScale = 0.2;
	static const float maxScale = 2.0;	
	static const float scaleDiffMax = 7;
	static const float scaleDurTime = 1000;
	
};