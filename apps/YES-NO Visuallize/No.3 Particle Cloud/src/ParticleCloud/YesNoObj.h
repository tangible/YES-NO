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
	void setup(ofxBullet* bl, AdminPanel* ap, int _YesOrNo, int _numObjs);
	void update();
	void updateColor();
	void draw();
	void computeCloudShape(Flock3D f);
	void computeMovement(Flock3D f);
	void addSMSObj(int size);
	void changeParticleObjNum(ofxBullet* bullet, int numPart);
	void changeSizeObjNum(ofxBullet* bullet, int numSize);
	
	int getBoidID() { return boidID; }
	int getObjNum() { return (int)objs.size(); }
	int getAddeObjNum() { return (int)addedObjs.size(); }
	
	float particleObjSize;
	float addedObjSize;
	float impulseFactor;
	float colorRadius;

private:
	void drawObjs();
	void drawAddedObjs();
	void sortByDistanceFromSMSObj();
	
	ofxBullet* bullet;
	AdminPanel* adminPanel;
	
	int YesOrNo;
	float baseMin; // if yes => 0.0, no => 0.5
	float baseMax; // if yes => 0.5, no => 1.0
	int boidID;	
	
	// objs fields
	int numObjs;
	vector<Obj> objs;
	vector<int> sortedObjIdxByDistanceFromSMS;
	vector<Obj> addedObjs;	
	vector<int> sortedAddedObjIdxByDistanceFromSMS;	
	
};