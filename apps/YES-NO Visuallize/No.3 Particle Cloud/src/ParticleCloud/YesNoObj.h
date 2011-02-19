/*
 *  YesNoObj.h
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
#include "Obj.h"
#include "ofxColorPicker.h"

class YesNoObj {

public:
	void setup(ofxBullet* bl, AdminPanel* ap, int _YesOrNo);
	void update();
	void draw();
	void computeShape(vector<ofxVec3f> posFeed);
	
	
	ofxBullet* bullet;
	AdminPanel* adminPanel;
	ofxColorPicker colPick;	
	
	int YesOrNo;
	float baseMin; // if yes => 0.0, no => 0.5
	float baseMax; // if yes => 0.5, no => 1.0
	
	// objs fields
	int numObjs;
	vector<Obj> objs;

};