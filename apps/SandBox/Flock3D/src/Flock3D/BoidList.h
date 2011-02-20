/*
 *  BoidList.h
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "Boid.h"

class BoidList {

public:
	void setup(float _width, float _height, float _near, float _far, int n, float ih);
	void add();
	void addBoid(Boid* b);
	void update(bool aW);
	void draw();
	vector<ofxVec3f> getTrailPoints(int boidIdx);
	
	float width;
	float height;
	float near;
	float far;
	vector<Boid*> boids;
	float h;

};