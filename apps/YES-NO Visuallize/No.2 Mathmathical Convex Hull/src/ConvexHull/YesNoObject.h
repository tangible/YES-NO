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
#include "ofxColorPicker.h"

class Vertex {

public:
	MyRigidBody* sms;
	ofxColorPicker col;
	float angleMin;
	float angleMax;
	
	void update() {
//		if (col.getColorAngle() >= angleMax) {
//			col.setColorAngle(col.getColorAngle()-0.01);		
//		}else if (col.getColorAngle() <= angleMin) {
//			col.setColorAngle(col.getColorAngle()+0.01);		
//		}
		//cout << "col.getColorAngle() = " + ofToString(col.getColorAngle()) << endl;
		col.update();
	}
	
};

class YesNoObject {
	
public:
	
	void setup(ofxBullet* _bullet, ofxVec3f _forcePoint, int initialNumObj);
	void update();
	void draw(ofxVec4f col);
	void debugDraw();
	
	void addSMS(int numSMS, int YesOrNo = 0, ofxVec3f pos = ofxVec3f(0, 0, 0));
	void calcFaceColor(float* points, vector<int> faceDef, float* colPtrRtn);
	
	ofxBullet* bullet;
	
	int YesOrNo;
	float maxValu;
	ofxVec3f forcePoint;
	vector<Vertex*> smss;	
	
	Chull3D* ch3d;
	
};