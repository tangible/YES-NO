/*
 *  Flock3D.h
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "BoidList.h"
#include "ofx3DUtils.h"

class Flock3D {

public:
	void setup(int numFlocks, float _width, float _height, float _near, float _far, int _initBoidNum);
	void update(bool avoidWall);
	void draw();
	vector<ofxVec3f> getTrailPoints(int flockIdx);
	
	float width;
	float height;
	float near;
	float far;
	int initBoidNum;
	vector<BoidList*> flocks;

};