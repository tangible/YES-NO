/*
 *  Obj.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "Obj.h"

void Obj::setup(int yesOrNo, ofxBullet* bullet, ofxVec3f pos, int radius, int mass, 
				float _forceVecFactor,
				float _tangentVecFactor) {

	forceVecFactor = _forceVecFactor;
	tangentVecFactor = _tangentVecFactor;
	
	if (yesOrNo == 0) {
		body = bullet->createSphere(pos,
									radius, 
									mass);
	}else {
		body = bullet->createBox(pos,
									ofxVec3f(radius,radius,radius), 
									mass);
	}
}

void Obj::movetoForcePoint(int impulseFactor) {

	float maxVal = 0.01;
	ofxVec3f force;
	ofxVec3f crossVec;
	crossVec.set(0, 0, 0);
	crossVec.rotate(ofGetFrameNum(), ofGetFrameNum(), ofGetFrameNum());
	ofxVec3f tangentVec;
	
	// apply force
	force.set(-body->getBodyPos() + forcePoint);
	force *= maxVal * forceVecFactor;
	tangentVec = force.crossed(crossVec);
	tangentVec.normalize();
	tangentVec *= maxVal*tangentVecFactor;
	force += tangentVec;
	btVector3 btImpulse(force.x, force.y, force.z);
	body->getRigidBody()->applyCentralImpulse(btImpulse);
	
	ofxVec3f impulse;
	impulse.set(ofRandomf(), ofRandomf(), ofRandomf());
	impulse *= impulseFactor;
	
	btImpulse = btVector3(impulse.x, impulse.y, impulse.z);
	body->getRigidBody()->applyCentralImpulse(btImpulse);	

}
