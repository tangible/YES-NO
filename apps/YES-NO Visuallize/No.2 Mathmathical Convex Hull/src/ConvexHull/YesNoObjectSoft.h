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


class YesNoObjectSoft {

public:
	
	void setup(int _yesOrNo, ofxBullet* _bullet, ofxVec3f _forcePoint, ofxVec3f scale, int _sizeLevel);
	void update();
	void draw();
	void debugDraw();
	vector<float> changeColBySMSRecievedFace(int z);
	vector<int> sortFaceByDistance(btSoftBody::tFaceArray& faces, int faceID);
	ofxVec3f getFaceCentroid(btSoftBody::tFaceArray& faces, int faceID);	
	float getFaceDistanceBetween(btSoftBody::tFaceArray& faces, int face1ID, int face2ID);
	ofxVec3f getFaceNormal(btSoftBody::tFaceArray& faces, int face1ID);
	ofxVec3f getObjCentroid(btSoftBody::tFaceArray& faces);
	void updateColorPointer();
	void startFaceingToCam(ofxCamera* cam, ofxVec3f offset);
	void updateRotateion();
	
	void addSMS(int faceID, int _numSMS, float _ratioSMS);
	void addSMSCompleted(int & z);
	void blowUp(float pinchFaceFactor);
	void shrink();
	void pinchAndSpreadNode();
	void expandFace(int faceIdx);
	void pinchFace(int face);
	void pinchNode();
	
	void clear();
	
	int incomingSMSFaceID;	
	int numSMS;
	float ratioSMS;
	
	ofEvent<int> onFinishAllUpdating;
	void notifyFinishAllUpdating(int & z);
	
	ofxBullet*	bullet;
	MySoftBody*	yesORno;
	ofxColorPicker col;	
	float defaultColAng;
	float previousColAng;
	
	static const int YES = 0;
	static const int NO = 0;
	static const int minRes = 16;
	static const int maxRes = 5120;
	static const int YNSOFTMINSIZELEV = 0;
	static const int YNSOFTMAXSIZELEV = 20;	
	static const int maxSMSNum = 5000;
	int resolusion;	
	ofxVec3f radius;
	int sizeLevel;
	
	vector<float> destColorPointer;
	vector<float> currColorPointer;
	ofxTween	  colorPointerTween;
	ofxEasingCirc easing;
	
	vector<addedSMS*> addedSMSs;
	
	int			YesOrNo;
	ofxVec3f	forcePoint;
	
	ofxTween facingTween;
	ofxEasingCubic facingEasing;
	ofxVec3f prevFacingAxis;
	float prevFaceAngle;
	
	static const float maxNumScale = 2.0;
	static const float maxRatioScale = 2.0;
	static const float minScale = 0.8;
	static const float maxScale = 2.0;
	float curScale;
	ofxTween scaleTween;
	ofxEasingBounce scaleEasing;
	void setCurrentScale(int & z);
	float getScale();
	
};