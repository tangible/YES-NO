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

class IncomingSMS {
	
public:
	
	void setup(btSoftBody::tFaceArray faces);
	void update(ofxVec3f destination);
	void draw();
	void debugDraw();
	
	ofEvent<int> onSmsReached;
	bool	bNotify;
	
	int faceID;
	
	ofxTween distanceTween;
	ofxEasingCirc circ;
	
	btSoftBody::tFaceArray* faces;
	btVector3* btnormal;
	btSoftBody::Node* node0;
	btSoftBody::Node* node1;
	btSoftBody::Node* node2;	
	
};