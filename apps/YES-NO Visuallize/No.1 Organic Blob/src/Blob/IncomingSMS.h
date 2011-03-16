/*
 *  IncomingSMS.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/26.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once 

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxColorPicker.h"
#include "ofxTween.h"

class IncomingSMS {

public:
	
	IncomingSMS(){};
	~IncomingSMS() { body->remove(bullet->getWorld()); };
	
	void setup(ofxBullet* _bullet, int _chunkID, float colAng);
	void update(ofxVec3f tgtPos, float rad);
	void draw();
	
	int chunkID;
	ofxBullet* bullet;
	MyRigidBody* body;
	ofxColorPicker color;
	ofColor actualCol;	
	float size;
	float deceleration;

	
	ofEvent<int> onBallGetSMSrepEvent;
	ofEvent<int> onBallGetSMSrepCompleteEvent;	

//	ofxTween posTween;
//	ofxEasingCubic posEasing;
};