/*
 *  Boid.h
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofx3DUtils.h"

class Boid {

public:
	void setup(float _width, float _height, float _near, float _far, ofxVec3f inPos);
	void setup(float _width, float _height, float _near, float _far, ofxVec3f inPos, ofxVec3f inVel, float r);
	void update(vector<Boid*> bl);
	void draw();
	void flock(vector<Boid*> bl);	
	void move();
	void checkBounds();
	ofxVec3f avoid(ofxVec3f target, bool weight);
	ofxVec3f alignment(vector<Boid*> boids);
	ofxVec3f cohesion(vector<Boid*> boids);
	ofxVec3f seperation(vector<Boid*> boids);

	
	float width;
	float height;
	float near;
	float far;
	ofxVec3f pos;
	ofxVec3f vel;
	ofxVec3f acc;
	ofxVec3f ali;
	ofxVec3f coh;
	ofxVec3f sep;
	float neighborhoodRadius;
	float maxSpeed;
	float maxSteerForce;
	//float h;
	float sc;
	float flap;
	float t;
	bool avoidWalls;
	
};