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
#include "ofxTween.h"
#include "VerticeContainer.h"

class ParticleCloud {

public:
	void setup(int _fps, AdminPanel* ap, ofxCamera* cam);
	void update();
	void draw();
	void debugKeyPress(int key);
	void feedSMS(UpdateInfo upInfo);
	
	// yes/no obj
	YesNoObj yes;
	YesNoObj no;	
	
	float getSMSColorYes() { return yes.getSMSColor(); }
	float getSMSColorNo() { return no.getSMSColor(); }	
	
	
private:	
	
	void setupGLStuff();
	
	int fps;
	ofxBullet* bullet;
	AdminPanel* adminPanel;
	ofxColorPicker colPick;
	UpdateInfo updateInfo;
	
	ofxTween yesScaleTween;
	ofxTween noScaleTween;
	ofxEasingElastic scaleEasing;	
	float currYesScale;
	float currNOScale;	
	static const float minScale = 1.0;
	static const float maxScale = 1.5;	
	static const float scaleDiffMax = 7;
	static const float scaleDurTime = 1000;
	
	// sounds
	ofSoundPlayer			soundYes;
	ofSoundPlayer			soundNo;	
	
	// verts
	VerticeContainer vcon;
	
	// trails
	Flock3D flockYes;
	Flock3D flockNo;	
	
	// internal param
	static const float particleNumDiffMax = 7;
	static const float minParticleNum = 40;
	static const float maxParticleNum = 700;
	static const float minSizeObjNumPc = 1;
	static const float maxSizeObjNumPc = 400;
	static const float minSizeObjSize = 10;
	static const float maxSizeObjSize = 60;
	static const float minImpulseFactor = 20;
	static const float maxImpulseFactor = 70;
	static const float maxBoidSpeed = 6.5;
	static const float minBoidSpeed = 2.5;
	static const float normaBoidSpeed = 4.0;
	
};