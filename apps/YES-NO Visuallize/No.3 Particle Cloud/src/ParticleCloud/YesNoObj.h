/*
 *  YesNoObj.h
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "AdminPanel.h"
#include "Obj.h"
#include "Flock3D.h"

class YesNoObj {

public:
	void setup(ofxBullet* bl, Flock3D* f, AdminPanel* ap, int _YesOrNo, int _numObjs);
	void update();
	void draw();
	void computeCloudShape();
	void computeMovement();
	void addSMSObj(int size, bool bInitial = false);
	void changeParticleObjNum(ofxBullet* bullet, int numPart);
	void changeSizeObjNum(ofxBullet* bullet, int numSize);
	ofxVec3f getCentroid();
	vector<int> sortObjFromSMS(ofxVec3f smsPos);
	void updateColorByDistance(ofxVec3f smsPos);
	
	void onNotifyCollideSMS(int & z);
	
	ofEvent<int> notifyUpdateStextColorYesEvent;
	ofEvent<int> notifyUpdateStextColorNoEvent;	
	ofEvent<int> notifyStartStextFadingYesEvent;
	ofEvent<int> notifyStartStextFadingNoEvent;		
	
	int getBoidID() { return boidID; }
	int getObjNum() { return (int)objs.size(); }
	int getAddeObjNum() { return (int)addedObjs.size(); }
	void drawFlock() { flock->draw(); }
	Flock3D* getFlock() { return flock; }
	
	float getSMSColor() { return addedObjs[addedObjs.size()-1]->colAngle; };
	
	float particleObjSize;
	float addedObjSize;
	float impulseFactor;
	float initialColAngle;
	
	static const float minSizeObjNum = 0;
	static const float maxSizeObjNum = 400;
	static const float minParticleSize = 10;
	static const float maxParticleSize = 30;
	static const float minParticleMass = 5;
	static const float maxParticleMass = 10;
	static const float minForce = 55;
	static const float maxForce = 1000;
	static const float minTangent = 100;
	static const float maxTangent = 2000;	

private:
	void drawObjs();
	void drawAddedObjs();
	
	ofxBullet* bullet;
	AdminPanel* adminPanel;
	Flock3D* flock;	
	
	int YesOrNo;
	float baseMin; // if yes => 0.0, no => 0.5
	float baseMax; // if yes => 0.5, no => 1.0
	int boidID;	
	
	// objs fields
	int numObjs;
	vector<Obj*> objs;
	vector<Obj*> addedObjs;	
	
};