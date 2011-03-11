/*
 *  VerticeContainer.h
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/03/10.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofx3DUtils.h"

class VerticeContainer {
public:
	vector<ofxVec3f> vertices;
	void createSphericalVertice(int resolution = 30, float size = 100);
	vector<ofxVec3f> getVerts() {return vertices;};

};