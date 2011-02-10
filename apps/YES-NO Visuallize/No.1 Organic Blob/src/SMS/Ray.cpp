/*
 *  Ray.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/07.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "Ray.h"

void Ray::setup(ofxVec3f _centroid) {

	p1 = ofxVec3f(ofRandom(-200, ofGetWidth()+200), 1000, 0);
	p2 = ofxVec3f(ofRandom(p1.x-100, p1.x+100), ofRandom(p1.y-100, p1.y+100), 0);
	p3 = p1;
	
	postw.setParameters(bnc, ofxTween::easeIn, p3.x, centroid.x, 0, 0);
	postw.addValue(p3.y, centroid.y);
	postw.addValue(p3.z, centroid.z);	
	
	centroid = _centroid;

}

bool Ray::update(ofxVec3f _centroid) {

//	if (postw.isCompleted()) {
//		postw.setParameters(bnc, ofxTween::easeIn, p3.x, centroid.x, 0, 0);
//		postw.addValue(p3.y, centroid.y);
//		postw.addValue(p3.z, centroid.z);		
//	}
	centroid = _centroid;
	postw.start();
	postw.update();
	
	p3 = ofxVec3f(postw.update(), postw.getTarget(1), postw.getTarget(2));
	
//	if (p3.distance(centroid) < 50) {
//		return true;
//	}else {
//		return false;
//	}

}


void Ray::draw() {

	ofSetColor(255, 255, 255);
	ofxTriangleShape(p1, p2, p3);

}