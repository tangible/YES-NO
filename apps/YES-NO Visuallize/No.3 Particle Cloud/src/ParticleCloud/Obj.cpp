/*
 *  Obj.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "Obj.h"

void Obj::setup(ofxBullet* bullet, ofxVec3f pos, int radius, int mass) {

	body = bullet->createSphere(pos,
											 radius, 
											 mass, 
											 ofxVec4f(ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), 0.7), 
											 DYNAMIC_BODY);	

}

void Obj::movetoForcePoint() {

	float maxVal = 0.01;
	ofxVec3f force;
	ofxVec3f crossVec;
	crossVec.set(0, 0, 0);
	crossVec.rotate(ofGetFrameNum(), ofGetFrameNum(), ofGetFrameNum());
	ofxVec3f tangentVec;
	
	// apply force
	force.set(-body->getBodyPos() + forcePoint);
	force *= maxVal * 60;
	tangentVec = force.crossed(crossVec);
	tangentVec.normalize();
	tangentVec *= maxVal*100;
	force += tangentVec;
	btVector3 btImpulse(force.x, force.y, force.z);
	body->getRigidBody()->applyCentralImpulse(btImpulse);
	
	ofxVec3f impulse;
	impulse.set(ofRandomf(), ofRandomf(), ofRandomf());
	impulse *= 40;
	
	btImpulse = btVector3(impulse.x, impulse.y, impulse.z);
	body->getRigidBody()->applyCentralImpulse(btImpulse);	

}