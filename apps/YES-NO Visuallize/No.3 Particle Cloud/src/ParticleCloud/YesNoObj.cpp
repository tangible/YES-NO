/*
 *  YesNoObj.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "YesNoObj.h"

void YesNoObj::setup(ofxBullet* bl, AdminPanel* ap, int _YesOrNo) {
	
	bullet = bl;
	adminPanel = ap;
	YesOrNo = _YesOrNo;
	
	baseMin = (YesOrNo == Obj::YES) ? 0.0 : 0.5;
	baseMax = (YesOrNo == Obj::YES) ? 0.5 : 1.0;
	
	numObjs = 200;
	for (int i = 0; i < numObjs; i++) {
		float size = ofRandom(10, 20);
		float cAng = ofRandom(baseMin, baseMax);
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
		
		ofxQuaternion qt = obj.getObjRot();
		float rAng; ofxVec3f axis;
		qt.getRotate(rAng, axis);
		ofPushMatrix();
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotate(ofRadToDeg(rAng), axis.x, axis.y, axis.z);
		ofxSphere(0,0,0, size);		
//		ofxBox(0,0,0, size);
		ofPopMatrix();
		
	}
	ofSetColor(255, 255, 255);
	
}

void YesNoObj::computeShape(vector<ofxVec3f> posFeed) {

	float threeOfFour = 0.7;
	ofxVec3f tofVec = posFeed[ofMap(threeOfFour, 0.0, 1.0, 0, posFeed.size())];
	vector<ofxVec3f> branNewPos;
	for (int i = 0; i < posFeed.size(); i++) {
		ofxVec3f tmpVec = posFeed[i];
		ofxVec3f resVec = tmpVec.getInterpolated(tofVec, 0.3);
		branNewPos.push_back(resVec);
	}
	for (int i = 0; i < branNewPos.size(); i++) {
		ofxVec3f tp = branNewPos[i];
		objs[i].setForcePoint(tp);
	}	
	
//	for (int i = 0; i < posFeed.size(); i++) {
//		ofxVec3f tp = posFeed[i];
//		objs[i].setForcePoint(tp);
//	}
	
	
//	for (int i = 0; i < 1; i++) {
//		if (posFeed.size() > 0) {
//			ofxVec3f tp = posFeed[i];
//			for (int j = 0; j < objs.size(); j++) {
//				objs[j].setForcePoint(tp);
//			}
//		}
//	}
	
//	for (int i = 0; i < objs.size(); i++) {
//		ofxVec3f fp;
//		ofxVec3f tp = posFeed[i];
//		fp = tp;
//		objs[i].setForcePoint(fp);
//	}

}

