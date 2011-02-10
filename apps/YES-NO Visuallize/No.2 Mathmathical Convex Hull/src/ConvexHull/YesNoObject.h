/*
 *  YesNoObject.h
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/10.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "chull.h"

class YesNoObject {
	
public:
	
	void setup(ofxBullet* _bullet, ofxVec3f _forcePoint, int initialNumObj);
	void update();
	void draw(ofxVec4f col);
	void debugDraw();
	
	void addSMS(int numSMS, ofxVec3f pos = ofxVec3f(0, 0, 0));
	
	ofxBullet* bullet;
	
	int YesOrNo;
	float maxValu;
	ofxVec3f forcePoint;
	vector<MyRigidBody*> smss;	
	map<string, ofxVec4f> faceColMap;
	
	Chull3D* ch3d;
	
};