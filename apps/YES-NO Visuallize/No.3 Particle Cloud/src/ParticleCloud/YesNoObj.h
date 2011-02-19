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
	void setup(ofxBullet* bl, AdminPanel* ap);
	void update();
	void draw();
	
	ofxBullet* bullet;
	AdminPanel* adminPanel;
	ofxColorPicker colPick;	
	
	// objs fields
	int numObjs;
	vector<Obj> objs;
//	vector<MyRigidBody*> objs;
//	vector<int> objSize;	
//	vector<float> colAngle;
//	vector<ofxVec3f> forcePoint;
//	float colScale;
//	float colRadius;	

};