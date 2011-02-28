/*
 *  IncomingSMS.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/26.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "IncomingSMS.h"

void IncomingSMS::setup(ofxBullet* _bullet, int _chunkID, float colAng) {

	bullet = _bullet;
	chunkID = _chunkID;
	bool up = (ofRandomf() > 0) ? true : false;
	bool left = (ofRandomf() > 0) ? true : false;
	bool far = (ofRandomf() > 0) ? true : false;
//	ofxVec3f rdmPos = ofxVec3f(ofRandom((left)?0:ofGetScreenWidth()*2, (left)?-ofGetScreenWidth():ofGetScreenWidth()),
//							   ofRandom((up)?0:ofGetScreenHeight()*2, (left)?-ofGetScreenHeight():ofGetScreenHeight()),
//							   1300);
	ofxVec3f rdmPos = ofxVec3f(ofRandom(-300, 300),
							   ofRandom(-300, 300),
							   1300);
	
	
	MyRigidBody* sph = bullet->createSphere(rdmPos,
											10, 
											1, ofxVec4f(0,0,0,1.0),
											KINEMATIC_BODY);
	size = ofRandom(20, 40);
	
	color.setColorScale(1.0);
	color.setColorRadius(1.0);
	color.setColorAngle(colAng+0.5); // opposite side of color circle
	color.update();
	actualCol = color.getColor();
	
	deceleration = ofRandom(0.97, 0.98);
	
	body = sph;

}


void IncomingSMS::update(ofxVec3f tgtPos) {

	ofPoint spherePos = body->getBodyPos();
	float ampX = tgtPos.x - spherePos.x;
	float ampY = tgtPos.y - spherePos.y;
	float ampZ = tgtPos.z - spherePos.z;
	float tmpAmpX = ampX * deceleration;
	float tmpAmpY = ampY * deceleration;
	float tmpAmpZ = ampZ * deceleration;
	spherePos.x += ampX - tmpAmpX;
	spherePos.y += ampY - tmpAmpY;
	spherePos.z += ampZ - tmpAmpZ;	
	body->translateBody(spherePos, body->rotDir, ofGetFrameNum());
	
	
//	int numManifolds = bullet->getWorld()->getDispatcher()->getNumManifolds();
//	for (int i = 0; i<numManifolds-1; i++) {
//		btPersistentManifold* contactManifold =  bullet->getWorld()->getDispatcher()->getManifoldByIndexInternal(i);
//		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
//		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
//
//		if (obA == body->getRigidBody()) {
//			ofNotifyEvent(onBallGetSMSrepEvent, chunkID);
//		}else if (obB == body->getRigidBody()) {
//			ofNotifyEvent(onBallGetSMSrepEvent, chunkID);
//		}
//
//	}		
	
	if (100 > tgtPos.distance(spherePos)) {
		ofNotifyEvent(onBallGetSMSrepEvent, chunkID);
	}
	
}

void IncomingSMS::draw() {

	ofxVec3f pos = body->getBodyPos();
	ofxSphere(pos.x, pos.y, pos.z, 100);

}