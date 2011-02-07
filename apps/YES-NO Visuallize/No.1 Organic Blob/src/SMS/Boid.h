/*
 *  Boid.h
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#ifndef BOID_H
#define BOID_H

#include "ofMain.h"
#include "ofxVectorMath.h"

class Boid {
public:
	Boid(int maxSpeed = 20, float maxForce = 3.0);
		
	void update(vector<Boid*> boids);
	void draw();
	void seek(ofPoint target);
	void arrive(ofPoint target);
	ofPoint steer(ofPoint target, bool slowdown);
	
	void flock(vector<Boid*> boids);
	ofPoint separate(vector<Boid*> boids);
	ofPoint align(vector<Boid*> boids);
	ofPoint cohesion(vector<Boid*> boids);
	
	ofxVec3f loc;
	ofPoint vel;
	ofPoint acc;

	float r;
	float maxforce;
	float maxspeed;
};

#endif