/*
 *  Boid.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "Boid.h"

void Boid::setup(float _width, float _height, float _near, float _far, ofxVec3f inPos,
				 float _maxSpeed, float _maxSteerForce, int _posSize) {

	width = _width;
	height = _height;
	near = _near;
	far = _far;
	maxSpeed = _maxSpeed;
	maxSteerForce = _maxSteerForce;
	sc = 3;
	flap = 0;
	t = 0;
	avoidWalls = false;
	posSize = _posSize;
	pos = inPos;
	vel = ofxVec3f(ofRandomf(), ofRandomf(), ofRandomf());
	acc = ofxVec3f(0, 0, 0);
	neighborhoodRadius = 100000;
	
}

void Boid::update(vector<Boid*> bl) {

	t += 0.1;
	flap = 10*sin(t);
	if (avoidWalls) {
		acc += avoid(ofxVec3f(pos.x, height, pos.z), true)*5;
		acc += avoid(ofxVec3f(pos.x, 0, pos.z), true)*5;
		acc += avoid(ofxVec3f(width, pos.y, pos.z), true)*5;
		acc += avoid(ofxVec3f(0, pos.y, pos.z), true)*5;
		acc += avoid(ofxVec3f(pos.x, pos.y, far), true)*5;
		acc += avoid(ofxVec3f(pos.x, pos.y, near), true)*5;		
	}
	flock(bl);
	move();
	checkBounds();
	
	if (poss.size() < posSize) {
		poss.push_back(pos);
	}else {
		poss.erase(poss.begin());
		poss.push_back(pos);
	}
	
	
}

void Boid::draw() {
	
	ofPushMatrix();
	ofTranslate(pos.x, pos.y, pos.z);
	ofRotateY(ofRadToDeg(atan2(-vel.z, vel.x)));
	ofRotateZ(ofRadToDeg(asin(vel.y/vel.length())));
	
	ofxTriangleShape(3*sc,0,0, -3*sc,2*sc,0, -3*sc,-2*sc,0);
	ofxTriangleShape(3*sc,0,0, -3*sc,2*sc,0, -3*sc,0,2*sc);
	ofxTriangleShape(3*sc,0,0, -3*sc,0,2*sc, -3*sc,-2*sc,0);
	ofxTriangleShape(-3*sc,0,2*sc, -3*sc,2*sc,0, -3*sc,-2*sc,0);
	
	ofPopMatrix();
	
	for (int i = 0; i < poss.size(); i++) {
		ofxPoint(poss[i]);
	}
	
}

void Boid::flock(vector<Boid*> bl) {

    ali = alignment(bl);
    coh = cohesion(bl);
    sep = seperation(bl);
	acc += ali*1;
	acc += coh*3;
	acc += sep*1;

}

void Boid::move() {

	vel += acc;
	vel.limit(maxSpeed);
	pos += vel;
	acc *= 0;
	
}

void Boid::checkBounds() {
	
	if (pos.x > width) pos.x = 0;
	if (pos.x < 0) pos.x = width;
	if (pos.y > height) pos.y = 0;
	if (pos.y < 0) pos.y = height;
	if (pos.z > near) pos.z = far;
	if (pos.z < far) pos.z = near;
	
}

ofxVec3f Boid::avoid(ofxVec3f target, bool weight) {
	
    ofxVec3f steer; //creates vector for steering
    steer = pos-target; //steering vector points away from target
    if(weight) {
		float dist = pos.distance(target);
		steer *= 1.0/(dist*dist);
	}
    return steer;
	
}

ofxVec3f Boid::alignment(vector<Boid*> boids) {
	
    ofxVec3f velSum = ofxVec3f(0,0,0);
    int count = 0;
    for(int i=0;i<boids.size();i++) {
		Boid* b = boids[i];
		float d = pos.distance(b->pos);
		if(d > 0 && d <= neighborhoodRadius) {
			velSum += b->vel;
			count++;
		}
    }
    if (count > 0) {
		velSum /= (float)count;
		velSum.limit(maxSteerForce);
    }
    return velSum;	
	
}

ofxVec3f Boid::cohesion(vector<Boid*> boids) {
	
	ofxVec3f posSum = ofxVec3f(0,0,0);
	ofxVec3f steer = ofxVec3f(0,0,0);
	int count = 0;
	for (int i = 0; i < boids.size(); i++) {
		Boid* b = boids[i];
		ofxVec2f posv = ofxVec2f(pos.x, pos.y);
		ofxVec2f posb = ofxVec2f(b->pos.x, b->pos.y);
		float d = posv.distance(posb);
		if (d > 0 && d <= neighborhoodRadius) {
			posSum += b->pos;
			count++;
		}
	}
	if (count > 0) {
		posSum /= count;
	}
	steer = posSum-pos;
	steer.limit(maxSteerForce);
	return steer;
	
}

ofxVec3f Boid::seperation(vector<Boid*> boids) {

	ofxVec3f posSum = ofxVec3f(0,0,0);
	ofxVec3f repulse;
	for (int i = 0; i < boids.size(); i++) {
		Boid* b = boids[i];
		float d = pos.distance(b->pos);
		if (d > 0 && d <= neighborhoodRadius) {
			repulse = pos-b->pos;
			repulse.normalize();
			repulse /= d;
			posSum += repulse;
		}
	}
	return posSum;
	
}