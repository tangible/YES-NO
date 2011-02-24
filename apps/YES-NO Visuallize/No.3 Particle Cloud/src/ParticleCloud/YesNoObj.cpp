/*
 *  YesNoObj.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "YesNoObj.h"

void YesNoObj::setup(ofxBullet* bl, AdminPanel* ap, int _YesOrNo, int _numObjs) {
	
	bullet = bl;
	adminPanel = ap;
	YesOrNo = _YesOrNo;
	
	baseMin = (YesOrNo == Obj::YES) ? 0.0 : 0.5;
	baseMax = (YesOrNo == Obj::YES) ? 0.5 : 1.0;
	boidID = (YesOrNo == Obj::YES) ? 0 : 0;
	
	numObjs = _numObjs;
	for (int i = 0; i < numObjs; i++) {
		float size = ofRandom(10, 20);
		float cAng = ofRandom(baseMin, baseMax);
//		ofxVec3f rdmPos = ofxVec3f(ofGetWidth()/2+ofRandom(500, 200), ofGetHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));
		ofxVec3f rdmPos = ofxVec3f(ofGetScreenWidth()/2+ofRandom(500, 200), ofGetScreenHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));		
		Obj obj;
		obj.setup(bullet, rdmPos, 11, 1);
		obj.size = size;
		obj.colAngle = cAng;
//		obj.setForcePoint(ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
		obj.setForcePoint(ofxVec3f(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0));		
		objs.push_back(obj);
		
	}
	
	for (int i = 0; i < 1; i++) {
		addSMSObj(60);
	}
	addedObjSize = 1.0;
	particleObjSize = 1.0;
	impulseFactor = 20;
	colorRadius = 0.0;
}

void YesNoObj::update() {
	
	if (adminPanel->TOGGLEMOTION) {
		for (int i = 0; i < objs.size(); i++) {
			objs[i].movetoForcePoint(impulseFactor);
		}
		for (int i = 0; i < addedObjs.size(); i++) {
			addedObjs[i].movetoForcePoint();
		}
	}
	
}

void YesNoObj::draw() {

	drawObjs();
	drawAddedObjs();
	
}

void YesNoObj::computeCloudShape(Flock3D f) {
	
	vector<ofxVec3f> posFeed = f.getTrailPoints(YesOrNo, boidID);
	float threeOfFour = 0.6;
	ofxVec3f tofVec = posFeed[ofMap(threeOfFour, 0.0, 1.0, 0, posFeed.size())];
	
	float compDist = 0.0;
	for (int i = 0; i < posFeed.size(); i++) {
		ofxVec3f tmpVec = posFeed[i];
		float dist = tmpVec.distance(tofVec);
		if (dist > compDist) {
			compDist = dist;
		}
	}
	
	// make obj more cloudy
	for (int i = 0; i < posFeed.size(); i++) {
		
		// pos
		ofxVec3f tmpVec = posFeed[i];
		float dist = tmpVec.distance(tofVec);
		float compare = (dist >= compDist) ? dist+0.1 : compDist;
		float iplFactor = ofMap(dist, 0.0, compare, 1.0, 0.3);
		ofxVec3f resVec = tmpVec.getInterpolated(tofVec, iplFactor);
		objs[i].setForcePoint(resVec);
		
		// size
		float tmpSize = objs[i].size;
		float sizeFactor = ofMap(dist, 0.0, compare, 1.5, 1.0);
		objs[i].positionalSizeFactor = sizeFactor;
		
	}
	
	for (int i = 0; i < addedObjs.size(); i++) {
		
		// pos
		addedObjs[i].setForcePoint(tofVec);
		
		// size
		addedObjs[i].positionalSizeFactor = 0.7;
	}
}

void YesNoObj::computeMovement(Flock3D f) {
	
//	BoidList* bl = f.flock1;
//	vector<Boid*> boids = bl->boids;
//	float speed = ofRandom(4, 10);
//	boids[boidID]->maxSpeed = speed;
//	float force = ofRandom(0.01, 0.07);
//	boids[boidID]->maxSteerForce = force;	
	
}

void YesNoObj::addSMSObj(int size) {
	
	float cAng = ofRandom(baseMin, baseMax);
	
	bool up = (ofRandomf() > 0) ? true : false;
	bool left = (ofRandomf() > 0) ? true : false;
	bool far = (ofRandomf() > 0) ? true : false;
//	ofxVec3f rdmPos = ofxVec3f(ofRandom((left)?0:ofGetWidth(), (left)?0:ofGetWidth()),
//							   ofRandom((up)?0:ofGetHeight(), (left)?0:ofGetHeight()),
//							   -700);
	ofxVec3f rdmPos = ofxVec3f(ofRandom((left)?0:ofGetScreenWidth(), (left)?0:ofGetScreenWidth()),
							   ofRandom((up)?0:ofGetScreenHeight(), (left)?0:ofGetScreenHeight()),
							   -700);	
	
	
	Obj obj;
	obj.setup(bullet, rdmPos, 26, 350, 14500, 19000);
	obj.size = size;
	obj.colAngle = cAng;
//	obj.setForcePoint(ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
	obj.setForcePoint(ofxVec3f(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0));	
	addedObjs.push_back(obj);	
	
}

void YesNoObj::changeParticleObjNum(ofxBullet* bullet, int numPart) {
	
	if (numPart < objs.size()) {
		int diff = objs.size()-numPart;
		for (int i = 0; i < diff; i++) {
			int rdm = ofRandom(0, objs.size());
			objs[rdm].getBody()->remove(bullet->getWorld());
			objs.erase(objs.begin()+rdm);
		}
	}else {
		int diff = numPart-objs.size();
		for (int i = 0; i < diff; i++) {
			float size = ofRandom(10, 20);
			float cAng = ofRandom(baseMin, baseMax);
//			ofxVec3f rdmPos = ofxVec3f(ofGetWidth()/2+ofRandom(500, 200), ofGetHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));
			ofxVec3f rdmPos = ofxVec3f(ofGetScreenWidth()/2+ofRandom(500, 200), ofGetScreenHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));			
			Obj obj;
			obj.setup(bullet, rdmPos, 11, 1);
			obj.size = size;
			obj.colAngle = cAng;
//			obj.setForcePoint(ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, 0));
			obj.setForcePoint(ofxVec3f(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0));			
			objs.push_back(obj);
		}	
	}
	
}

void YesNoObj::changeSizeObjNum(ofxBullet* bullet, int numSize) {
	
	if (numSize < addedObjs.size()) {
		int diff = addedObjs.size()-numSize;
		for (int i = 0; i < diff; i++) {
			int rdm = ofRandom(0, addedObjs.size());
			addedObjs[rdm].getBody()->remove(bullet->getWorld());
			addedObjs.erase(addedObjs.begin()+rdm);
		}
	}
	
}

void YesNoObj::drawObjs() {
	
	ofSetColor(255, 255, 255);
	for (int i = 0; i < objs.size(); i++) {
		Obj obj = objs[i];
		float sizeBase = adminPanel->sizeBase;
		float size = obj.size;
		size += sizeBase;
		size *= obj.positionalSizeFactor;
		size *= particleObjSize;
		ofxVec3f pos = obj.getObjPos();
		float cAng = obj.colAngle;
		obj.colp.setColorAngle(-cAng);//cAng);//0.32);
		obj.colp.setColorScale(adminPanel->colScale);
		obj.colp.setColorRadius(adminPanel->colRadius+colorRadius);
		obj.colp.update();
		ofColor col = obj.colp.getColor();
		ofSetColor(col.r, col.g, col.b);
		
		ofxQuaternion qt = obj.getObjRot();
		float rAng; ofxVec3f axis;
		qt.getRotate(rAng, axis);
		
		ofPushMatrix();
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotate(ofRadToDeg(rAng), axis.x, axis.y, axis.z);
		
		(YesOrNo == Obj::YES) ? ofxSphere(0,0,0, size*1.225) : ofxBox(0,0,0, size*1.6);
		
		ofPopMatrix();
		
	}
	ofSetColor(255, 255, 255);	
	
}

void YesNoObj::drawAddedObjs() {
	
	ofSetColor(255, 255, 255);
	for (int i = 0; i < addedObjs.size(); i++) {
		Obj obj = addedObjs[i];
		float sizeBase = adminPanel->sizeBase;
		float size = obj.size;
		size += sizeBase;
		size *= obj.positionalSizeFactor;
		size *= addedObjSize;
		ofxVec3f pos = obj.getObjPos();
		float cAng = obj.colAngle;
		obj.colp.setColorAngle(-cAng);//cAng);//0.32);
		obj.colp.setColorScale(adminPanel->colScale);
		obj.colp.setColorRadius(adminPanel->colRadius+colorRadius);
		obj.colp.update();
		ofColor col = obj.colp.getColor();
		ofSetColor(col.r, col.g, col.b);
		
		ofxQuaternion qt = obj.getObjRot();
		float rAng; ofxVec3f axis;
		qt.getRotate(rAng, axis);
		
		ofPushMatrix();
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotate(ofRadToDeg(rAng), axis.x, axis.y, axis.z);
		
		(YesOrNo == Obj::YES) ? ofxSphere(0,0,0, size) : ofxBox(0,0,0, size*1.5);
		
		ofPopMatrix();
		
	}	
	ofSetColor(255, 255, 255);	
	
}
