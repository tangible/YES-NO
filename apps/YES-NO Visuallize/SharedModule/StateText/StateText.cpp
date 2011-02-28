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
	
	font.loadFont("Verdana Bold.ttf", 200, true, true, true);
	
	dfont.loadFont("Helvetica.dfont", 200, true, true, true);
	
	yes = "yes";
	no = "no";
	yesNum = "0";
	noNum = "0";
	yesSize = 0.1;
	noSize = 0.1;
	
	yesXTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);
	yesYTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);
	noXTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);
	noYTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);
	yesSizeTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);
	noSizeTween.setParameters(easingcirc, ofxTween::easeIn, 0.0, 0.0, 0, 0);

	yesSize = 0.3;
	noSize = 0.3;
//	yesNum = "122";
//	noNum = "560";
}

void StateText::onSMSReceivedUpdate(int yesORno, UpdateInfo upInfo) {
	
	if (yesORno == 0) {
		yesNum = ofToString(upInfo.numTotalYes);
		yesSize = (upInfo.numTotalYes == 0)?0.3:ofMap(upInfo.ratioTotalYes, 0.0, 1.0, 0.1, 1.0);
		yesSizeTween.setParameters(easingcirc, ofxTween::easeIn, yesSize, yesSize*1.5, 100, 0);			
	}else {	
		noNum = ofToString(upInfo.numTotalNo);		
		noSize = (upInfo.numTotalNo == 0)?0.3:ofMap(upInfo.ratioTotalNo, 0.0, 1.0, 0.1, 1.0);	
		noSizeTween.setParameters(easingcirc, ofxTween::easeIn, noSize, noSize*1.5, 100, 0);	
		
	}
	
}

void StateText::draw(UpdateInfo upInfo, ofxVec3f centroidYes, ofxVec3f centroidNo) {
	
	if (yesSizeTween.isCompleted()) {
		yesSizeTween.setParameters(easingcirc, ofxTween::easeIn, yesSizeTween.getTarget(0), yesSize, 100, 0);
	}
	if (noSizeTween.isCompleted()) {
		noSizeTween.setParameters(easingcirc, ofxTween::easeIn, noSizeTween.getTarget(0), noSize, 100, 0);	
	}
	
	ofxVec3f lineDistY = centroidYes;
	
	float yoff = 200;
	float xoff = 200;
	float sizeYes = yesSizeTween.update();
	float sizeStr = font.stringWidth(yes+ofToString(upInfo.numTotalYes))*yesSize;
	float strHeight = font.stringHeight(yes)*yesSize;
	if (centroidYes.y < ofGetScreenHeight()/2-200) {
		centroidYes.y += yoff;
		strHeight *= -1;
	}else {
		centroidYes.y -= yoff;
		strHeight *= 0;
		strHeight += 5;
	}
	if (centroidYes.x < ofGetScreenWidth()/2) {
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
	ofSetColor(0, 0, 0);
	float widYes = font.stringWidth(yes);
	font.drawStringAsShapes(yesNum, widYes*1.1, 0);
	ofPopMatrix();
	
	ofxLine(transY.x+sizeStr/2, transY.y+strHeight, 0.0, lineDistY.x, lineDistY.y, lineDistY.z);
	
	
	ofxVec3f lineDistN = centroidNo;
	
	float sizeNo = noSizeTween.update();
	sizeStr = font.stringWidth(no+ofToString(upInfo.numTotalNo))*noSize;
	strHeight = font.stringHeight(no)*sizeNo;
	if (centroidNo.y < ofGetScreenHeight()/2-200) {
		centroidNo.y += yoff;
		strHeight *= -1;
	}else {
		centroidNo.y -= yoff;
		strHeight *= 0;
		strHeight += 5;		
	}	
	if (centroidNo.x < ofGetScreenWidth()/2) {
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
	ofSetColor(0, 0, 0);
	float widNo = font.stringWidth(no);
	font.drawStringAsShapes(noNum, widNo*1.1, 0);
	ofPopMatrix();	

	ofxLine(transN.x+sizeStr/2, transN.y+strHeight, 0.0, lineDistN.x, lineDistN.y, lineDistN.z);
}

void StateText::draw(UpdateInfo upInfo) {
	
	glDisable(GL_LIGHTING);
	ofEnableSmoothing();

	if (yesSizeTween.isCompleted()) {
		yesSizeTween.setParameters(easingcirc, ofxTween::easeIn, yesSizeTween.getTarget(0), yesSize, 100, 0);
	}
	if (noSizeTween.isCompleted()) {
		noSizeTween.setParameters(easingcirc, ofxTween::easeIn, noSizeTween.getTarget(0), noSize, 100, 0);	
	}	
	
	float sizeYes = yesSizeTween.update();	
	float widYesNum = font.stringWidth(yesNum);
	float heiYesNum = font.getLineHeight()*sizeYes*0.8;
	float bottom = ofGetScreenHeight()-50;
	float left = 30;
	float numOffset = -32*sizeYes;
	ofPushMatrix();
	ofSetColor(255, 255, 255);	
	ofTranslate(left, bottom-heiYesNum, 0);
	ofScale(sizeYes*0.3, sizeYes*0.3, 0);	
	dfont.drawString(yes, 0, 0);
	ofPopMatrix();
	ofPushMatrix();
	ofSetColor(255, 255, 255);
	ofTranslate(left+numOffset, bottom, 0);
	ofScale(sizeYes, sizeYes, 0);	
	dfont.drawString(yesNum, 0, 0);
	ofPopMatrix();
	
	float sizeNo = noSizeTween.update();
	float widNoNum = font.stringWidth(noNum)*sizeNo+60;
	float heiNoNum = font.getLineHeight()*sizeNo*0.8;
	left = ofGetScreenWidth()-widNoNum;
	numOffset = -32*sizeNo;
	ofPushMatrix();
	ofSetColor(255, 255, 255);
	ofTranslate(left, bottom-heiNoNum, 0);
	ofScale(sizeNo*0.3, sizeNo*0.3, 0);
	dfont.drawString(no, 0, 0);
	ofPopMatrix();
	ofPushMatrix();
	ofSetColor(255, 255, 255);
	ofTranslate(left+numOffset, bottom, 0);
	ofScale(sizeNo, sizeNo, 0);
	dfont.drawString(noNum, 0, 0);
	ofPopMatrix();
	
	glEnable(GL_LIGHTING);
	
//	ofPushMatrix();
//	ofScale(0.5, 0.5, 1.0);
//	dfont.drawString("012abscTTTA", 100, 100);
//	ofPopMatrix();
}

