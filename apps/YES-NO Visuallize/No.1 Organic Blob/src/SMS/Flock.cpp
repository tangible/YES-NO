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
		//cout << ofToString(birdLoc.distance(mouseLoc)) << endl;
		//cout << "num birds = "+ofToString((int)boids.size()) << endl;
		if (50 > birdLoc.distance(mouseLoc)) {
			
			if (1 > boids.size()) {
				//ofNotifyEvent(onBallGetSMSrepCompleteEvent, flockID);
			}			
			
			boids.erase(boids.begin()+i);
			ofNotifyEvent(onBallGetSMSrepEvent, flockID);
		}

		
		boids[i]->update(boids);
	}
}

void Flock::draw() {
	for(int i=0; i<boids.size(); i++) {
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