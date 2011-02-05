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
		
		ofxVec3f birdLoc = boids[i]->loc;
		ofxVec3f mouseLoc = ofxVec3f(mx, my, 0);
		cout << ofToString(birdLoc.distance(mouseLoc)) << endl;
		if (20.0 > birdLoc.distance(mouseLoc)) {
			boids.erase(boids.begin()+i);
		}
		
		boids[i]->update(boids);
	}
}

void Flock::draw() {
	for(int i=0; i<boids.size(); i++) {
		cout << ofToString(cols[i].r) << endl;
		ofSetColor(cols[i].r, cols[i].g, cols[i].b);		
		boids[i]->draw();
		ofSetColor(255, 255, 255);
	}
}

void Flock::addBoid() {
	Boid* boid = new Boid();
	ofColor col;
	col.r = ofRandom(0, 255);
	col.g = ofRandom(0, 255);
	col.b = ofRandom(0, 255);
	cols.push_back(col);
	//boid->col = col;
	boids.push_back( new Boid() );
}