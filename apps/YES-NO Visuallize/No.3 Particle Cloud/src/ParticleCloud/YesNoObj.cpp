/*
 *  YesNoObj.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "YesNoObj.h"

void YesNoObj::setup(ofxBullet* bl, AdminPanel* ap) {
	
	bullet = bl;
	adminPanel = ap;
	
	numObjs = 200;
	for (int i = 0; i < numObjs; i++) {
		float size = ofRandom(10, 20);
		float cAng = ofRandomuf();
		ofxVec3f rdmPos = ofxVec3f(ofGetWidth()/2+ofRandom(500, 200), ofGetHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));
		Obj obj;
		obj.setup(bullet, rdmPos, 10, 1);
		obj.size = size;
		obj.colAngle = cAng;
		obj.setForcePoint(ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
		objs.push_back(obj);
		
	}
	
}

void YesNoObj::update() {
	
	if (adminPanel->TOGGLEMOTION) {
		for (int i = 0; i < objs.size(); i++) {
			objs[i].movetoForcePoint();
		}
	}
	
}

void YesNoObj::draw() {
	
	ofSetColor(255, 255, 255);
	for (int i = 0; i < objs.size(); i++) {
		Obj obj = objs[i];
		float sizeBase = adminPanel->sizeBase;
		float size = obj.size;
		size += sizeBase;
		ofxVec3f pos = obj.getObjPos();
		float cAng = obj.colAngle;
		colPick.setColorAngle(cAng);
		colPick.setColorScale(adminPanel->colScale);
		colPick.setColorRadius(adminPanel->colRadius);
		colPick.update();
		ofColor col = colPick.getColor();
		ofSetColor(col.a, col.g, col.b);
		ofxSphere(pos.x, pos.y, pos.z, size);
		
//		ofxQuaternion qt = obj.getObjRot();
//		float rAng; ofxVec3f axis;
//		qt.getRotate(rAng, axis);
//		ofPushMatrix();
//		ofTranslate(pos.x, pos.y, pos.z);
//		ofRotate(ofRadToDeg(rAng), axis.x, axis.y, axis.z);
//		ofxBox(0,0,0, size);
//		ofPopMatrix();
		
	}
	ofSetColor(255, 255, 255);
	
}