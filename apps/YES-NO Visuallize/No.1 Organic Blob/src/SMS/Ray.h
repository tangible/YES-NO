/*
 *  Ray.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/07.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxTween.h"
#include "ofx3DUtils.h"

class Ray {

public:
	void setup(ofxVec3f centroid);
	bool update(ofxVec3f centroid);
	void draw();
	
	ofxVec3f p1;
	ofxVec3f p2;
	ofxVec3f p3;
	
	ofxVec3f centroid;
	
	ofxTween postw;
	ofxEasingBounce bnc;
	
};