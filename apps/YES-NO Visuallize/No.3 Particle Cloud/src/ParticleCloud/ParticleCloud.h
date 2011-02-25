/*
 *  ParticleCloud.h
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "AdminPanel.h"
#include "ofxColorPicker.h"
#include "YesNoObj.h"
#include "Flock3D.h"
#include "HTTPSMSClient.h"

class ParticleCloud {

public:
	void setup(int _fps, AdminPanel* ap, ofxCamera* cam);
	void update();
	void draw();
	void debugKeyPress(int key);
	void feedSMS(UpdateInfo upInfo);
	
private:	
	int fps;
	ofxBullet* bullet;
	AdminPanel* adminPanel;
	ofxColorPicker colPick;

	// yes/no obj
	YesNoObj yes;
	YesNoObj no;
	
	// trails
	Flock3D flockYes;
	Flock3D flockNo;	
	
	// internal param
	static const float minParticleNum = 50;
	static const float maxParticleNum = 600;
	static const float minSizeObjNum = 1;
	static const float maxSizeObjNum = 20;
	static const float minSizeObjSize = 10;
	static const float maxSizeObjSize = 60;
	static const float minImpulseFactor = 20;
	static const float maxImpulseFactor = 70;
	static const float maxBoidSpeed = 6.5;
	static const float minBoidSpeed = 2.5;
	static const float normaBoidSpeed = 4.0;
	
};