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

void Flock::update(int mx, int my) {
	for(int i=0; i<boids.size(); i++) {

		if(i<5) {
			boids[i]->seek( ofPoint(mx, my) );
		} else {
			boids[i]->arrive( ofPoint(mx, my) );
		}		
		
		ofxVec2f birdLoc = ofxVec2f(boids[i]->loc.x, boids[i]->loc.y);
		ofxVec2f mouseLoc = ofxVec2f(mx, my);

		if (50 > birdLoc.distance(mouseLoc)) {		
			Boid* b = boids[i];
			delete b;
			boids.erase(boids.begin()+i);
			ofNotifyEvent(onBallGetSMSrepEvent, flockID);
		}

		
		boids[i]->update(boids);
	}
}

void Flock::draw() {
	for(int i=0; i<boids.size(); i++) {
		boids[i]->draw();
	}
}

void Flock::addBoid() {
	Boid* boid = new Boid();
	boids.push_back(boid);
}