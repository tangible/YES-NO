/*
 *  Effects.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/05.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxTween.h"

class Effects {
public:
	void setup();
	void draw();
	
	ofxTween colTween;
	ofxTween sizeTween;
	
	vector<ofImage> imgs;

};