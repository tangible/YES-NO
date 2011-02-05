/*
 *  Effects.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/05.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "Effects.h"

int numImgs = 1;

void Effects::setup() {

	for (int i = 1; i <= numImgs; i++) {
		ofImage img;
		img.loadImage(ofToString(i)+".png");
		imgs.push_back(img);
	}

}

void Effects::draw() {

	for (int i = 0; i < imgs.size(); i++) {
		
		ofImage img = imgs[i];
		img.draw(0, 0);
		
	}
}