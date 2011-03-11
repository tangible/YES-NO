/*
 *  VerticeContainer.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/03/10.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "VerticeContainer.h"

void VerticeContainer::createSphericalVertice(int resolution, float size) {

	float theta,nextTheta, phi, x, y, z;
	for(int i=0; i<resolution; i++){
		theta = (float)i/(resolution-1) * TWO_PI;
		nextTheta = (float)((i+1)%resolution)/(resolution-1) * TWO_PI;
		for(int j=0; j<resolution; j++){
			phi = (float)j/(resolution-1) * PI;
			x = cos(theta) * sin(phi);
			y = sin(theta) * sin(phi);
			z = cos(phi);
			ofxVec3f a(x,y,z);
			a *= size;
			vertices.push_back(a);
			x = cos(nextTheta) * sin(phi);
			y = sin(nextTheta) * sin(phi);
			z = cos(phi);
			ofxVec3f b(x,y,z);
			b *= size;
			vertices.push_back(b);			
		}
	}	

}