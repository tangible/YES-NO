/*
 *  YesNoObj.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "YesNoObj.h"

void YesNoObj::setup(ofxBullet* bl, Flock3D* f, AdminPanel* ap, int _YesOrNo, int _numObjs) {
	
	bullet = bl;
	adminPanel = ap;
	YesOrNo = _YesOrNo;
	flock = f;
	
	baseMin = (YesOrNo == Obj::YES) ? -0.34 : -0.02;
	baseMax = (YesOrNo == Obj::YES) ? -0.34 : -0.02;
	boidID = (YesOrNo == Obj::YES) ? 0 : 0;
	
	numObjs = 1;//_numObjs;
//	for (int i = 0; i < numObjs; i++) {
//		float size = ofRandom(minParticleSize, maxParticleSize);
//		float cAng = ofRandom(baseMin, baseMax);
//		ofxVec3f rdmPos = ofxVec3f(ofGetScreenWidth()/2+ofRandom(500, 200), ofGetScreenHeight()/2+ofRandom(-200, 200), ofRandom(-500, 0));
//		Obj* obj = new Obj();
//		
//		float mass = ofMap(size, minParticleSize, maxParticleSize, minParticleMass, maxParticleMass);
//		float force = ofMap(size, minParticleSize, maxParticleSize, minForce, maxForce);
//		float tangent = ofMap(size, minParticleSize, maxParticleSize, minTangent, maxTangent);
//		
//		obj->setup(YesOrNo, bullet, rdmPos, 11, mass, force, tangent);
//		obj->size = size;
//		obj->colAngle = cAng;
//		obj->colScale = 1.0;
//		obj->colRadius = 1.0;
//		obj->setForcePoint(ofxVec3f(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0));
//		objs.push_back(obj);
//		
//	}
	
	for (int i = 0; i < 1; i++) {
		addSMSObj(40, true);
	}
	addedObjSize = 1.0;
	particleObjSize = 1.0;
	impulseFactor = 20;
	initialColAngle = ofRandom(baseMin, baseMax);
	
}

void YesNoObj::update() {
	
	if (adminPanel->TOGGLEMOTION) {
		for (int i = 0; i < objs.size(); i++) {
			objs[i]->movetoForcePoint(0);
		}
		for (int i = 0; i < addedObjs.size(); i++) {
			if (i == addedObjs.size()-1) {
				addedObjs[i]->movetoForcePoint(0, true);
			}else {
				addedObjs[i]->movetoForcePoint(0);
			}
		}
	}
	
	flock->update(true);
}

void YesNoObj::draw() {

	drawObjs();
	drawAddedObjs();
	
}

void YesNoObj::computeCloudShape() {
	
	vector<ofxVec3f> posFeed = flock->getTrailPoints(0, 0);
	int p = (int)posFeed.size();
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
	ofxVec3f pos;
	for (int i = 0; i < objs.size(); i++) {
		
		// pos
		ofxVec3f tmpVec = posFeed[i];
		float dist = tmpVec.distance(tofVec);
		float compare = (dist >= compDist) ? dist+0.1 : compDist;
		float iplFactor = ofMap(dist, 0.0, compare, 1.0, 0.7);
		ofxVec3f resVec = tmpVec.getInterpolated(tofVec, iplFactor);
		
		
		if (YesOrNo == 0) {
			pos = ofxVec3f(ofGetScreenWidth()/3, ofGetScreenHeight()/2, 0);
		}else if (YesOrNo == 1) {
			pos = ofxVec3f(ofGetScreenWidth()-ofGetScreenWidth()/3, ofGetScreenHeight()/2, 0);
		}
		objs[i]->setForcePoint(pos);
		
//		objs[i].setForcePoint(resVec);
		
		// size
		float tmpSize = objs[i]->size;
		float sizeFactor = ofMap(dist, 0.0, compare, 1.5, 1.0);
		objs[i]->positionalSizeFactor = sizeFactor;
		
	}
	
	for (int i = 0; i < addedObjs.size(); i++) {
		
		if (YesOrNo == 0) {
			pos = ofxVec3f(ofGetScreenWidth()/3, ofGetScreenHeight()/2, 0);
		}else if (YesOrNo == 1) {
			pos = ofxVec3f(ofGetScreenWidth()-ofGetScreenWidth()/3, ofGetScreenHeight()/2, 0);
		}		
		// pos
		addedObjs[i]->setForcePoint(pos);
		
		// size
		addedObjs[i]->positionalSizeFactor = 0.7;
	}
}

void YesNoObj::computeMovement() {
	
//	BoidList* bl = f.flock1;
//	vector<Boid*> boids = bl->boids;
//	float speed = ofRandom(4, 10);
//	boids[boidID]->maxSpeed = speed;
//	float force = ofRandom(0.01, 0.07);
//	boids[boidID]->maxSteerForce = force;	
	
}

void YesNoObj::addSMSObj(int size, bool bInitial) {
	
//	float cAng = ofRandom(baseMin, baseMax);
	initialColAngle += 0.01;
	float cAng = initialColAngle;
	
	bool up = (ofRandomf() > 0) ? true : false;
	bool left = (ofRandomf() > 0) ? true : false;
	bool far = (ofRandomf() > 0) ? true : false;
	
	ofxVec3f rdmPos;
	if (bInitial) {
		if (YesOrNo == 0) {
			rdmPos = ofxVec3f(ofGetScreenWidth()/3, ofGetScreenHeight()/2, 0);
		}else if (YesOrNo == 1) {
			rdmPos = ofxVec3f(ofGetScreenWidth()-ofGetScreenWidth()/3, ofGetScreenHeight()/2, 0);
		}	
	}else {
		rdmPos = ofxVec3f(ofRandom((left)?0:ofGetScreenWidth(), (left)?0:ofGetScreenWidth()),
						  ofRandom((up)?0:ofGetScreenHeight(), (left)?0:ofGetScreenHeight()),
						  1300);		
	}
	
	
	Obj* obj = new Obj();
	
	float mass = ofMap(addedObjs.size(), minSizeObjNum, maxSizeObjNum, 1, 100);
	float force = ofMap(addedObjs.size(), minSizeObjNum, maxSizeObjNum, 200, 700);
	float tang = ofMap(addedObjs.size(), minSizeObjNum, maxSizeObjNum, 300, 1000);
	
//	obj->setup(0, bullet, rdmPos, 30, 300, 24500, 19000);
//	obj->setup(0, bullet, rdmPos, size, 1, 500, 380);

	if (YesOrNo == 0) {
		obj->setup(YesOrNo, bullet, rdmPos, size, mass, force, tang);	
		obj->size = size;		
	}else {
		obj->setup(YesOrNo, bullet, rdmPos, size*0.8, mass, force, tang);	
		obj->size = size*0.8;
	}	
	
//	if (YesOrNo == 0) {
//		obj->setup(YesOrNo, bullet, rdmPos, size, mass, 500, 380);	
//		obj->size = size;		
//	}else {
//		obj->setup(YesOrNo, bullet, rdmPos, size*0.8, mass, 500, 380);	
//		obj->size = size*0.8;
//	}
	
	if (bInitial) {
		obj->colAngle = ofRandom(baseMin, baseMax);
	}else {
		obj->colAngle = cAng;
	}
	obj->colScale = 0.99;
	obj->colRadius = 0.99;
	obj->setForcePoint(ofxVec3f(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0));
	ofAddListener(obj->notifyCollideSMS, this, &YesNoObj::onNotifyCollideSMS);
	addedObjs.push_back(obj);	
	
}

void YesNoObj::changeParticleObjNum(ofxBullet* bullet, int numPart) {
	
	if (numPart < objs.size()) {
		int diff = objs.size()-numPart;
		for (int i = 0; i < diff; i++) {
			int rdm = ofRandom(0, objs.size());
			objs[rdm]->getBody()->remove(bullet->getWorld());
			objs.erase(objs.begin()+rdm);
		}
	}else {
		int diff = numPart-objs.size();
		for (int i = 0; i < diff; i++) {
			float size = ofRandom(minParticleSize, maxParticleSize);
			float cAng = ofRandom(baseMin, baseMax);
			
			vector<ofxVec3f> posFeed = flock->getTrailPoints(0, 0);
			int p = (int)posFeed.size();
			float threeOfFour = 0.6;
			ofxVec3f tofVec = addedObjs[addedObjs.size()-1]->getObjPos();
//			ofxVec3f rdmPos = ofxVec3f(tofVec.x+ofRandom(-400, 400), tofVec.y+ofRandom(-400, 400), tofVec.z+ofRandom(-400, 400));
			
			bool up = (ofRandomf() > 0) ? true : false;
			bool left = (ofRandomf() > 0) ? true : false;
			bool far = (ofRandomf() > 0) ? true : false;			
			ofxVec3f rdmPos = ofxVec3f(ofRandom((left)?0:ofGetScreenWidth(), (left)?0:ofGetScreenWidth()),
									   ofRandom((up)?0:ofGetScreenHeight(), (left)?0:ofGetScreenHeight()),
									   2300);			
			
			Obj* obj = new Obj();
			float mass = ofMap(size, minParticleSize, maxParticleSize, minParticleMass, maxParticleMass);
			float force = ofMap(size, minParticleSize, maxParticleSize, minForce, maxForce);
			float tangent = ofMap(size, minParticleSize, maxParticleSize, minTangent, maxTangent);			
			
			obj->setup(YesOrNo, bullet, rdmPos, 11, mass, force, tangent);
			obj->size = size;
			obj->colAngle = cAng;
			obj->setForcePoint(ofxVec3f(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 0));
			objs.push_back(obj);
		}	
	}
	
}

void YesNoObj::changeSizeObjNum(ofxBullet* bullet, int numSize) {
	
	if (numSize < addedObjs.size()) {
		int diff = addedObjs.size()-numSize;
		for (int i = 0; i < diff; i++) {
			int rdm = ofRandom(0, addedObjs.size());
			addedObjs[rdm]->getBody()->remove(bullet->getWorld());
			addedObjs.erase(addedObjs.begin()+rdm);
		}
	}
	
}

void YesNoObj::drawObjs() {
	
	ofSetColor(255, 255, 255);
	ofEnableSmoothing();
	for (int i = 0; i < objs.size(); i++) {
		Obj* obj = objs[i];
		float sizeBase = adminPanel->sizeBase;
		float size = obj->size;
		ofxVec3f pos = obj->getObjPos();
		float cAng = obj->colAngle;
		cAng += ofMap(size, minParticleSize, maxParticleSize, 0.2, 0.0);
		float cScale = ofMap(size, minParticleSize, maxParticleSize, 0.25, 1.0);
		float cRad = ofMap(size, minParticleSize, maxParticleSize, 1.0, 1.0);
		
		obj->colp.setColorAngle(cAng);//cAng);//0.32);
		obj->colp.setColorScale(cScale);
		obj->colp.setColorRadius(cRad);
		obj->colp.update();
		ofColor col = obj->colp.getColor();
		ofSetColor(col.r, col.g, col.b);
		
		ofxQuaternion qt = obj->getObjRot();
		float rAng; ofxVec3f axis;
		qt.getRotate(rAng, axis);
		
		size *= particleObjSize;
		
		ofPushMatrix();
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotate(ofRadToDeg(rAng), axis.x, axis.y, axis.z);
		
		(YesOrNo == Obj::YES) ? ofxSphere(0,0,0, size) : ofxBox(0,0,0, size*1.0);
		
		ofPopMatrix();
		
	}
	ofSetColor(255, 255, 255);	
	
}

void YesNoObj::drawAddedObjs() {
	
	//updateColorByDistance(addedObjs[addedObjs.size()-1]->getObjPos());
	
	ofSetColor(255, 255, 255);
	for (int i = 0; i < addedObjs.size(); i++) {
		Obj* obj = addedObjs[i];
		float sizeBase = adminPanel->sizeBase;
		float size = obj->size;
		ofxVec3f pos = obj->getObjPos();
		
//		obj->colp.setColorAngle(obj->colAngle);//cAng);//0.32);
//		obj->colp.setColorScale(0.99);
//		obj->colp.setColorRadius(0.99);
		
		if (i == addedObjs.size()-1) {
			obj->colp.setColorAngle(obj->colAngle);
			obj->colp.setColorScale(obj->colScale);
			obj->colp.setColorRadius(obj->colRadius);					
		}else {
			obj->colp.setColorAngle(obj->colorTween.update());
			obj->colp.setColorScale(obj->colorTween.getTarget(1));
			obj->colp.setColorRadius(obj->colorTween.getTarget(2));		
		}

		obj->colp.update();
		ofColor col = obj->colp.getColor();
		ofSetColor(col.r, col.g, col.b);
		
		ofxQuaternion qt = obj->getObjRot();
		float rAng; ofxVec3f axis;
		qt.getRotate(rAng, axis);
		
		ofPushMatrix();
		ofTranslate(pos.x, pos.y, pos.z);
		ofRotate(ofRadToDeg(rAng), axis.x, axis.y, axis.z);
		
		ofEnableSmoothing();
		ofEnableAlphaBlending();
		(YesOrNo == Obj::YES) ? ofxSphere(0,0,0, size*1.7) : ofxBox(0,0,0, size*2.4);
		
		ofPopMatrix();
		
	}	
	ofSetColor(255, 255, 255);	
	
}

ofxVec3f YesNoObj::getCentroid() {
	
	float x = 0.0; float y = 0.0; float z = 0.0;
	for (int i = 0; i < addedObjs.size(); i++) {
		ofxVec3f bpos = addedObjs[i]->getObjPos();
		x += bpos.x; y += bpos.y; z += bpos.z;
	}
	ofxVec3f cen;
	cen.x = x/addedObjs.size();
	cen.y = y/addedObjs.size();
	cen.z = z/addedObjs.size();
	return cen;
	
}

vector<int> YesNoObj::sortObjFromSMS(ofxVec3f smsPos) {
	
	// get Obj distance from smsPos
	map<float, int, greater<float> > distSort;
	for (int i = 0; i < addedObjs.size(); i++) {
		ofxVec3f cen = addedObjs[i]->getObjPos();
		float dist = cen.distance(smsPos);
		distSort.insert(map<float, int, greater<float> >::value_type(dist, i));
	}
	
	// sort dist desc
	vector<int> objIDVec;
	map<float, int, greater<float> >::iterator it = distSort.begin();
	while (it != distSort.end()) {
		objIDVec.push_back((*it).second);
		++it;
	}
	
	return objIDVec;
	
}

void YesNoObj::updateColorByDistance(ofxVec3f smsPos) {

	ofRemoveListener(addedObjs[addedObjs.size()-1]->notifyCollideSMS, this, &YesNoObj::onNotifyCollideSMS);
	
	float baseCol = addedObjs[addedObjs.size()-1]->colAngle;
	float angleStep = 0.5/addedObjs.size();
	float angleAccum = 0.0;
	vector<int> sortedVec = sortObjFromSMS(smsPos);
	for (int i = 0; i < sortedVec.size(); i++) {
		
		int idx = sortedVec[i];
		float colAng = ofMap(i, 0, sortedVec.size(), 0.0, 1.0);
		float colScl = ofMap(i, 0, sortedVec.size(), 0.2, 0.99);		
		float colRad = ofMap(i, 0, sortedVec.size(), 0.98, 0.98);
		
		angleAccum += angleStep;		
//		addedObjs[idx]->colAngle = initialColAngle+colAng;
		addedObjs[idx]->colAngle = baseCol+angleAccum;		
		addedObjs[idx]->colScale = colScl;
		addedObjs[idx]->colRadius = colRad;
		
		addedObjs[idx]->colorTween.setParameters(addedObjs[idx]->colorTweenEasing, ofxTween::easeIn, addedObjs[idx]->prevColAngle, addedObjs[idx]->colAngle, 1000, 0);
		addedObjs[idx]->colorTween.addValue(addedObjs[idx]->prevColScale, addedObjs[idx]->colScale);
		addedObjs[idx]->colorTween.addValue(addedObjs[idx]->prevColRadius, addedObjs[idx]->colRadius);		
	}
	
}

void YesNoObj::onNotifyCollideSMS(int & z) {
	
	updateColorByDistance(addedObjs[addedObjs.size()-1]->getObjPos());
	
}
