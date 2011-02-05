/*
 *  Flock.h
 *  flock
 *
 *  Created by Jeffrey Crouse on 3/30/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#ifndef FLOCK_H
#define FLOCK_H

#include "ofMain.h"
#include "Boid.h"

class Flock {
public:
	Flock();
	
	void update(int mx, int my);
	void draw();
	void addBoid();
	
	int flockID;
	int z;
	
	ofEvent<int> onBallGetSMSrepEvent;
	ofEvent<int> onBallGetSMSrepCompleteEvent;
	
	vector<Boid*> boids;
	vector<ofColor> cols;
};

#endif