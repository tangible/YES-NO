/*
 *  Boid.cpp
 *  boid
 *
 *  Created by Jeffrey Crouse on 3/29/10.
 *  Copyright 2010 Eyebeam. All rights reserved.
 *
 */

#include "Boid.h"

Boid::Boid() {

    loc.x = ofRandomWidth();
	loc.y = ofRandomHeight();
	vel = 0;
	acc = 0;
	
	cout << loc.x << ", " << loc.y << endl;
    r = 3.0;
    maxspeed = 4;
    maxforce = 0.1;
}


// Method to update location
void Boid::update() {
   
    vel += acc;   // Update velocity
    vel.x = ofClamp(vel.x, -maxspeed, maxspeed);  // Limit speed
	vel.y = ofClamp(vel.y, -maxspeed, maxspeed);  // Limit speed
    loc += vel;
    acc = 0;  // Reset accelertion to 0 each cycle
	
	if (loc.x < -r) loc.x = ofGetWidth()+r;
    if (loc.y < -r) loc.y = ofGetHeight()+r;
    if (loc.x > ofGetWidth()+r) loc.x = -r;
    if (loc.y > ofGetHeight()+r) loc.y = -r;
}

void Boid::seek(ofPoint target) {
    acc += steer(target, false);
}

void Boid::arrive(ofPoint target) {
    acc += steer(target, true);
}

// A method that calculates a steering vector towards a target
// Takes a second argument, if true, it slows down as it approaches the target
ofPoint Boid::steer(ofPoint target, bool slowdown) {
    ofPoint steer;  // The steering vector
    ofPoint desired = target - loc;  // A vector pointing from the location to the target
    float d = ofDist(target.x, target.y, loc.x, loc.y); // Distance from the target is the magnitude of the vector
    
	// If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0) {
		
		desired /= d; // Normalize desired
		// Two options for desired vector magnitude (1 -- based on distance, 2 -- maxspeed)
		if ((slowdown) && (d < 100.0f)) {
			desired *= maxspeed * (d/100.0f); // This damping is somewhat arbitrary
		} else {
			desired *= maxspeed;
		}
		// Steering = Desired minus Velocity
		steer = desired - vel;
		steer.x = ofClamp(steer.x, -maxforce, maxforce); // Limit to maximum steering force
		steer.y = ofClamp(steer.y, -maxforce, maxforce); 
    }
    return steer;
}

void Boid::draw() {
    // Draw a triangle rotated in the direction of velocity
    //float theta = vel.heading2D() + radians(90);
	
	/*
	ofPoint heading = loc + vel;  // A vector pointing from the location to where the boid is heading
    float d = ofDist(loc.x, loc.y, heading.x, heading.y); // Distance from the target is the magnitude of the vector
	heading /= d;
	*/
	
	float angle = (float)atan2(-vel.y, vel.x);
    float theta =  -1.0*angle;
	float heading2D = ofRadToDeg(theta)+90;
	
	ofEnableAlphaBlending();
    ofSetColor(0, 0, 0);
    ofFill();
    ofPushMatrix();
    ofTranslate(loc.x, loc.y);
    ofRotateZ(heading2D);
	ofBeginShape();
    ofVertex(0, -r*2);
    ofVertex(-r, r*2);
    ofVertex(r, r*2);
    ofEndShape(true);
    ofPopMatrix();
	ofDisableAlphaBlending();
}

