/*
 *  QuestionImage.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/06.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "QuestionImage.h"

void QuestionImage::setup() {

	qImg.loadImage("qimg.png");

}

void QuestionImage::draw() {

	// draw Question Img
	ofPushMatrix();
	ofSetupScreen();
	ofEnableAlphaBlending();
	qImg.draw(0, 0);
	ofDisableAlphaBlending();
	ofPopMatrix();

}

void QuestionImage::changeImgQImg(string path) {
	
	qImg.loadImage(path);
	
}
