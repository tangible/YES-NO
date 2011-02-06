/*
 *  QuestionImage.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/06.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class QuestionImage {

public:
	
	void setup();
	void draw();
	void changeImgQImg(string path);
	
	ofImage qImg;

};