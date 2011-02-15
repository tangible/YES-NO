/*
 *  IncomingSMS.h
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/11.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofx3DUtils.h"
#include "ofxVectorMath.h"
#include "ofxTween.h"
#include "ofxBullet.h"

class SmsInfo {
	
public:
	int YesOrNo;
	int faceID;
	ofxVec3f a;
	ofxVec3f b;
	ofxVec3f c;
	
};

class IncomingSMS {
	
public:
	
	void setup(int _YesOrNo, btSoftBody::tFaceArray faces, int _faceID);
	void update(btSoftBody::tFaceArray faces);
	void draw();
	void debugDraw();
	
	ofEvent<SmsInfo> onSmsReached;
	ofEvent<SmsInfo> onSmsCompleted;
	bool	bNotify;
	
	int YesOrNo;
	int faceID;
	
	ofxTween distanceTween;
	ofxEasingCirc circ;
	
	btSoftBody::tFaceArray* facesPtr;
	btVector3* btnormal;
	btSoftBody::Node* node0;
	btSoftBody::Node* node1;
	btSoftBody::Node* node2;	
	
};