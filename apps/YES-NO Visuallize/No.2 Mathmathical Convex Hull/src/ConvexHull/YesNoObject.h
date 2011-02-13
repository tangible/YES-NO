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
#include "IncomingSMS.h"

class Vertex {

public:
	MyRigidBody* sms;
	ofxColorPicker col;
	float angleMin;
	float angleMax;
	
	void update() {}
	
};

class YesNoObject {
	
public:
	
	void setup(ofxBullet* _bullet, ofxVec3f _forcePoint, int initialNumObj);
	void update();
	void draw();
	void debugDraw();
	
	void addSMS(int numSMS, int YesOrNo = 0, ofxVec3f pos = ofxVec3f(0, 0, 0));
	
	vector<float> caclColAngleAndScaleForObj(vector<float> faceVertexPtr, int nFaces);
	vector<float> setColPtr(ofColor tgtCol, vector<float> colPtr);
	Vertex* getVertexByPos(ofxVec3f pos);
	
	
	ofxBullet* bullet;
	
	int YesOrNo;
	float maxValu;
	ofxVec3f forcePoint;
	vector<Vertex*> smss;	
	
//	vector<IncomingSMS> insmss;
	IncomingSMS		insms;
	
	Chull3D* ch3d;
	
};