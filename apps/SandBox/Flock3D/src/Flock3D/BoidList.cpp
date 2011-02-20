/*
 *  BoidList.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "BoidList.h"

void BoidList::setup(float _width, float _height, float _near, float _far, int n, float ih) {
	
	width = _width;
	height = _height;
	near = _near;
	far = _far;
	h = ih;
	for (int i = 0; i < n; i++) {
		Boid* b = new Boid();
		b->setup(width, height, near, far, ofxVec3f(width/2, height/2, (near+far)/2));
		boids.push_back(b);
	}

}

void BoidList::add() {

	Boid* b = new Boid();
	b->setup(width, height, near, far, ofxVec3f(width/2, height/2, 0));
	boids.push_back(b);

}

void BoidList::addBoid(Boid* b) {

	boids.push_back(b);

}

void BoidList::update(bool aW) {

	for (int i = 0; i < boids.size(); i++) {
		Boid* tmpBoid = boids[i];
		//tmpBoid->h = h;
		tmpBoid->avoidWalls = aW;
		tmpBoid->update(boids);
	}

}

void BoidList::draw() {

	for (int i = 0; i < boids.size(); i++) {
		Boid* tmpBoid = boids[i];
		tmpBoid->draw();
	}

}

vector<ofxVec3f> BoidList::getTrailPoints(int boidIdx) {

	Boid* b = boids[boidIdx];
	return b->poss;

}