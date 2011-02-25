/*
 *  Obj.h
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxColorPicker.h"

class Obj {

public:
	void setup(int yesOrNo, ofxBullet* bullet, ofxVec3f pos, int radius, int mass, 
			   float _forceVecFactor = 55,
			   float _tangentVecFactor = 100);
	void setForcePoint(ofxVec3f fp){forcePoint = fp;}
	void movetoForcePoint(int impulseFactor = 40);
	
	MyRigidBody* getBody(){return body;}
	ofxVec3f getObjPos(){return body->getBodyPos();}
	ofxQuaternion getObjRot(){return body->getBodyRotQuat();}
	
	static const int YES = 0;
	static const int NO = 1;	
	static const float minParticleSize = 10;
	static const float maxParticleSize = 30;	
	
	int size;
	float colAngle;
	float colScale;
	float colRadius;
	ofxColorPicker colp;

	float positionalSizeFactor;
	float forceVecFactor;
	float tangentVecFactor;
	
private:
	MyRigidBody* body;
	ofxVec3f forcePoint;
	
};