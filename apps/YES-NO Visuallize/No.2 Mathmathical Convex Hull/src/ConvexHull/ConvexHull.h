/*
 *  ConvexHull.h
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/09.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "YesNoObject.h"
#include "LightScattering.h"
#include "ofxColor.h"

const int YES = 0;
const int NO = 1;

class ConvexHull {

public:
	
	void setup(int _fps, ofxCamera* _cam);
	void update();
	void draw();
	void keyPressed(int key);
	
	void setupGLStuff();
	
	float mc(float num);
	
	YesNoObject				yes;
	YesNoObject				no;
	
	int						fps;
	ofxBullet*				bullet;
	ofxCamera*				cam;
	
	LightScattering			lScat;
	
	GLfloat *materialAmbient;
	GLfloat *materialDiffuse;
	GLfloat *materialSpecular;	
	
};