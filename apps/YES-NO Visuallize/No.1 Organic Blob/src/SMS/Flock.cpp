/*
 *  Flock.cpp
 *  flock
 *
 *  Created by Jeffrey Crouse on 3/30/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#include "Flock.h"

Flock::Flock() {
}

void Flock::update(int mx, int my, int mz) {
	for(int i=0; i<boids.size(); i++) {

		if(i<5) {
			boids[i]->seek( ofPoint(mx, my, mz) );
		} else {
			boids[i]->arrive( ofPoint(mx, my, mz) );
		}		
		
		ofxVec2f birdLoc = ofxVec2f(boids[i]->loc.x, boids[i]->loc.y);
		ofxVec2f mouseLoc = ofxVec2f(mx, my);

		boids[i]->z = mz;
		
		boids[i]->update(boids);		
		
		if (50 > birdLoc.distance(mouseLoc)) {		
			Boid* b = boids[i];
			boids.erase(boids.begin()+i);
			delete b;			
			ofNotifyEvent(onBallGetSMSrepEvent, flockID);
		}

	}
}

ofxVec3f Flock::draw() {
	for(int i=0; i<boids.size(); i++) {
		return boids[i]->draw();
	}
}

void Flock::addBoid(int maxSpeed, float maxForce) {
	Boid* boid = new Boid(maxSpeed, maxForce);
	boids.push_back(boid);
}