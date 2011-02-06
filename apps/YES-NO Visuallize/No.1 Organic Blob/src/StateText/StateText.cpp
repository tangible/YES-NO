/*
 *  StateText.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/02/06.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "StateText.h"

void StateText::setup() {

	minSize = 10;
	maxSize = 100;
	
	font.loadFont("Verdana Bold.ttf", 10, true, true, true);
	
	yes = "YES:";
	no = "NO:";
	
	yesXTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);
	yesYTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);
	noXTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);
	noYTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);

}

void StateText::draw(UpdateInfo upInfo, ofxVec3f centroidYes, ofxVec3f centroidNo) {
	
	ofxVec3f lineDistY = centroidYes;
	
	
	float yoff = 200;
	float xoff = 200;
	float sizeYes = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, 1.0, 10.0);	
	float sizeStr = font.stringWidth(yes+ofToString(upInfo.numTotalYes))*sizeYes;
	float strHeight = font.stringHeight(yes)*sizeYes;
	if (centroidYes.y < ofGetHeight()/2-200) {
		centroidYes.y += yoff;
		strHeight *= -1;
	}else {
		centroidYes.y -= yoff;
		strHeight *= 0;
		strHeight += 5;
	}
	if (centroidYes.x < ofGetWidth()/2) {
		centroidYes.x += xoff-sizeStr/2;
	}else {
		centroidYes.x -= xoff+sizeStr/2;
	}
	
	yesXTween.setParameters(easingcirc, ofxTween::easeIn, yesXTween.update(), centroidYes.x, 100, 10);
	yesYTween.setParameters(easingcirc, ofxTween::easeIn, yesYTween.update(), centroidYes.y, 100, 10);
	ofxVec3f transY = ofxVec3f(yesXTween.update(), yesYTween.update(), 0);
	ofSetColor(121, 255, 0);
	ofPushMatrix();
	ofTranslate(transY.x, transY.y, transY.z);
	ofScale(sizeYes, sizeYes, 0);
	font.drawStringAsShapes(yes, 0, 0);
	ofSetColor(255, 255, 255);
	float widYes = font.stringWidth(yes);
	font.drawStringAsShapes(ofToString(upInfo.numTotalYes), widYes, 0);
	ofPopMatrix();
	
	ofxLine(transY.x+sizeStr/2, transY.y+strHeight, 0.0, lineDistY.x, lineDistY.y, lineDistY.z);
	
	
	ofxVec3f lineDistN = centroidNo;
	
	float sizeNo = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, 1.0, 10.0);	
	sizeStr = font.stringWidth(no+ofToString(upInfo.numTotalNo))*sizeNo;
	strHeight = font.stringHeight(no)*sizeNo;
	if (centroidNo.y < ofGetHeight()/2-200) {
		centroidNo.y += yoff;
		strHeight *= -1;
	}else {
		centroidNo.y -= yoff;
		strHeight *= 0;
		strHeight += 5;		
	}	
	if (centroidNo.x < ofGetWidth()/2) {
		centroidNo.x += xoff-sizeStr/2;
	}else {
		centroidNo.x -= xoff+sizeStr/2;
	}	
	
	noXTween.setParameters(easingcirc, ofxTween::easeIn, noXTween.update(), centroidNo.x, 100, 10);
	noYTween.setParameters(easingcirc, ofxTween::easeIn, noYTween.update(), centroidNo.y, 100, 10);	
	ofxVec3f transN = ofxVec3f(noXTween.update(), noYTween.update(), 0);
	ofSetColor(251, 0, 0);
	ofPushMatrix();
	ofTranslate(transN.x, transN.y, 0);
	ofScale(sizeNo, sizeNo, 0);
	font.drawStringAsShapes(no, 0, 0);
	ofSetColor(255, 255, 255);
	float widNo = font.stringWidth(no);
	font.drawStringAsShapes(ofToString(upInfo.numTotalNo), widNo, 0);
	ofPopMatrix();	

	ofxLine(transN.x+sizeStr/2, transN.y+strHeight, 0.0, lineDistN.x, lineDistN.y, lineDistN.z);
}
