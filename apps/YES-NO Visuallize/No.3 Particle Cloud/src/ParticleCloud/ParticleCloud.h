/*
 *  ParticleCloud.h
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "AdminPanel.h"
#include "ofxColorPicker.h"
#include "YesNoObj.h"
#include "Flock3D.h"

class ParticleCloud {

public:
	
	void setup(int _fps, AdminPanel* ap);
	void update();
	void draw();
	
	int fps;
	ofxBullet* bullet;
	AdminPanel* adminPanel;
	ofxColorPicker colPick;

	// yes/no obj
	YesNoObj yes;
	YesNoObj no;
	
	// trails
	Flock3D flock;
	
};