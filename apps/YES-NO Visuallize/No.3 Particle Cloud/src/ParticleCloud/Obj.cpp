/*
 *  Obj.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "Obj.h"

void Obj::setup(int yesOrNo, ofxBullet* _bullet, ofxVec3f pos, int radius, int mass, 
				float _forceVecFactor,
				float _tangentVecFactor) {

	bullet = _bullet;
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

void Obj::movetoForcePoint(int impulseFactor, bool bSMS) {

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
	
//	ofxVec3f impulse;
//	impulse.set(ofRandomf(), ofRandomf(), ofRandomf());
//	impulse *= impulseFactor;
//	
//	btImpulse = btVector3(impulse.x, impulse.y, impulse.z);
//	body->getRigidBody()->applyCentralImpulse(btImpulse);	
	
	if (bSMS) {
		int numManifolds = bullet->getWorld()->getDispatcher()->getNumManifolds();
		for (int i = 0; i < numManifolds; i++) {
			btPersistentManifold* contactManifold =  bullet->getWorld()->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
			btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

			if (obA == body->getRigidBody() || obB == body->getRigidBody()) {
				int z = 0;
				ofNotifyEvent(notifyCollideSMS, z);
			}

		}			
	}
	
	if (colorTween.isCompleted()) {
		prevColAngle = colAngle;
		prevColScale = colScale;
		prevColRadius = colRadius;
	}

}
