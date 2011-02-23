/*
 *  YesNoObjectSoft.h
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/12.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxColorPicker.h"
#include "ofxTween.h"
#include "ofxTimer.h"

class addedSMS {
	
public:
	int faceID;
	float length;
	btVector3* btnormal;
	btSoftBody::Node* node0;
	btSoftBody::Node* node1;
	btSoftBody::Node* node2;	
	float angle;
	ofxTween tw;
	ofxEasingCirc ea;

	ofxVec3f getA() {
		return ofxBulletStaticUtil::btVec3ToOfxVec3(node0->m_x);
	}
	ofxVec3f getB() {
		return ofxBulletStaticUtil::btVec3ToOfxVec3(node1->m_x);
	}
	ofxVec3f getC() {
		return ofxBulletStaticUtil::btVec3ToOfxVec3(node2->m_x);
	}
	ofxVec3f getNorm() {
		return ofxVec3f(btnormal->getX(), btnormal->getY(), btnormal->getZ());
	}
};

static const int YNSOFTMINSIZELEV = 0;
static const int YNSOFTMAXSIZELEV = 20;
class YesNoObjectSoft {

public:
	
	int resolusion;
	static const int minRes = 16;
	static const int maxRes = 5120;
	ofxVec3f radius;
	int sizeLevel;
	
	bool debugRun;

	int incomingSMSFaceID;
	
	void setup(int _yesOrNo, ofxBullet* _bullet, ofxVec3f _forcePoint, ofxVec3f scale, int _sizeLevel);
	void update();
	void draw();
	void debugDraw();
	vector<float>  changeColBySMSRecievedFace(btSoftBody::tFaceArray& faces, int faceID);
	vector<int> sortFaceByDistance(btSoftBody::tFaceArray& faces, int faceID);
	ofxVec3f getFaceCentroid(btSoftBody::tFaceArray& faces, int faceID);	
	float getFaceDistanceBetween(btSoftBody::tFaceArray& faces, int face1ID, int face2ID);
	void updateColorPointer();
	
	vector<float> destColorPointer;
	vector<float> currColorPointer;
	ofxTween	  colorPointerTween;
	ofxEasingCirc easing;
	
	vector<addedSMS*> addedSMSs;
	ofxTween shrinkTimer;
	TimedCounter* blowUpTimerr;
	ofxTween blowUpTimer;
	ofEvent<int> onTheEnd;
	void finish();
	bool bFinish;
	void clear();
	
	void debugKeyPressed(int key);
	
	void addSMS(int faceID);
	void addSMSCompleted(int faceID, vector<ofxVec3f> pos);
	void blowUp();
	void shrink();
	void pinchAndSpreadNode();
	void expandFace(int faceIdx);
	void pinchFace(int face);
	void pinchNode();
	
	ofxBullet*	bullet;
	MySoftBody*	yesORno;
	ofxColorPicker col;
	
	int			YesOrNo;
	float		maxValu;
	ofxVec3f	forcePoint;
	
};