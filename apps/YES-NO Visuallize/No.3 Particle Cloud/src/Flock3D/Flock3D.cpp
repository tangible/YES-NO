/*
 *  Flock3D.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "Flock3D.h"

void Flock3D::setup(float _width, float _height, float _near, float _far, int _initBoidNum) {
	
	width = _width;
	height = _height;
	near = _near;
	far = _far;
	initBoidNum = _initBoidNum;
	flock1 = new BoidList();
	flock1->setup(width, height, near, far, initBoidNum, 255);

}

void Flock3D::update(bool avoidWall) {
	
	flock1->update(avoidWall);
	
}

void Flock3D::draw() {

	ofSetColor(255, 255, 255);
	
	ofxLine(0,0,far,  0,height,far);
	ofxLine(0,0,near,  0,height,near);
	ofxLine(0,0,far,  width,0,far);
	ofxLine(0,0,near,  width,0,near);
	
	ofxLine(width,0,far,  width,height,far);
	ofxLine(width,0,near,  width,height,near);
	ofxLine(0,height,far,  width,height,far);
	ofxLine(0,height,near,  width,height,near);
	
	ofxLine(0,0,far,  0,0,near);
	ofxLine(0,height,far,  0,height,near);
	ofxLine(width,0,far,  width,0,near);
	ofxLine(width,height,far,  width,height,near);
	
	flock1->draw();
	
}

vector<ofxVec3f> Flock3D::getTrailPoints(int flockIdx) {

	return flock1->getTrailPoints(flockIdx);

}