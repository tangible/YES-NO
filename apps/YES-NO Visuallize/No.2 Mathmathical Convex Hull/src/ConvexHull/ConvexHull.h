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
#include "YesNoObject.h"
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
	
	int						fps;
	ofxBullet*				bullet;
	ofxCamera*				cam;	
	AdminPanel*				admin;
	
	YesNoObject				yes;
	YesNoObject				no;
	bool					isYesUpdating;
	bool					isNoUpdating;
	void					onFinishAllUpdating(int & yesOrNo);
	UpdateInfo				updateInfo;
	
	ofxVec3f				yesPoint;
	int						currentYesLevel;
	YesNoObjectSoft			yesSoft;
	ofxVec3f				noPoint;
	int						currentNoLevel;
	YesNoObjectSoft			noSoft;
	
	vector<IncomingSMS*>	insmsYes;
	vector<IncomingSMS*>	insmsNo;
	void onSmsReached(SmsInfo& smsInfo);
	void onSmsCompleted(SmsInfo& smsInfo);
	
};