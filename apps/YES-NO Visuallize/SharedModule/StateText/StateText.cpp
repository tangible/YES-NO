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
	
	dfont.loadFont("HelveticaBold.ttf", 200, true, true, true);
	
	numberFont.loadFont("HelveticaBold.ttf", numberFontSize, true, true, true);
	answerFont.loadFont("HelveticaBold.ttf", answerFontSize, true, true, true);
	
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
	
	
	yesColTween.setParameters(colTweenEasing, ofxTween::easeIn, 255.0, 255.0, 1000, 0);
	yesColTween.addValue(255.0, 255.0);
	yesColTween.addValue(255.0, 255.0);
	
	noColTween.setParameters(colTweenEasing, ofxTween::easeIn, 255.0, 255.0, 1000, 0);
	noColTween.addValue(255.0, 255.0);
	noColTween.addValue(255.0, 255.0);	
}

void StateText::onSMSReceivedUpdate(int yesORno, UpdateInfo upInfo) {
	
	if (yesORno == 0) {
		yesNum = ofToString(upInfo.numTotalYes);
		yesNum = insertComma(yesNum);
		yesSize = (upInfo.numTotalYes == 0)?0.3:ofMap(upInfo.ratioTotalYes, 0.0, 1.0, 0.1, 1.0);
		yesSizeTween.setParameters(easingcirc, ofxTween::easeIn, yesSize, yesSize*1.5, 100, 0);			
	}else {	
		noNum = ofToString(upInfo.numTotalNo);	
		noNum = insertComma(noNum);
		noSize = (upInfo.numTotalNo == 0)?0.3:ofMap(upInfo.ratioTotalNo, 0.0, 1.0, 0.1, 1.0);	
		noSizeTween.setParameters(easingcirc, ofxTween::easeIn, noSize, noSize*1.5, 100, 0);	
		
	}
	
}

void StateText::draw(UpdateInfo upInfo, ofxVec3f centroidYes, ofxVec3f centroidNo) {
	
	ofEnableSmoothing();
	
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
	ofEnableSmoothing();	
	ofScale(sizeYes*0.3, sizeYes*0.3, 0);
	ofEnableAlphaBlending(); 
	dfont.drawString(yes, 0, 0);
	ofDisableAlphaBlending(); 
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
	ofEnableSmoothing();	
	ofScale(sizeNo*0.3, sizeNo*0.3, 0);
	ofEnableAlphaBlending(); 
	dfont.drawString(no, 0, 0);
	ofDisableAlphaBlending(); 
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

void StateText::drawWithNoScale(UpdateInfo upInfo, float colAngYes, float colAngNo) {
	
	ofColor thisYesCol;
	ofColor thisNoCol;
	
	if (colAngYes != 1000.0) {
		colp.setColorAngle(colAngYes);
		colp.setColorScale(1.0);
		colp.setColorRadius(1.0);
		colp.update();
		thisYesCol = colp.getColor();
	}else {
		thisYesCol.r = 255.0; thisYesCol.g = 255.0; thisYesCol.b = 255.0;
	}
	
	if (colAngNo != 1000.0) {
		colp.setColorAngle(colAngNo);
		colp.setColorScale(1.0);
		colp.setColorRadius(1.0);
		colp.update();
		thisNoCol = colp.getColor();
	}else {
		thisNoCol.r = 255.0; thisNoCol.g = 255.0; thisNoCol.b = 255.0;
	}	
	
	drawWithNoScale(upInfo, thisYesCol, thisNoCol);
	
}

void StateText::drawWithNoScale(UpdateInfo upInfo) {
	
	ofColor _yesCol;
	ofColor _noCol;
	
	_yesCol.r = yesColTween.update();
	_yesCol.g = yesColTween.getTarget(1);
	_yesCol.b = yesColTween.getTarget(2);
	
	_noCol.r = noColTween.update();
	_noCol.g = noColTween.getTarget(1);
	_noCol.b = noColTween.getTarget(2);
	
	drawWithNoScale(upInfo, _yesCol, _noCol);
	
}

void StateText::drawWithNoScale(UpdateInfo upInfo, ofColor _yesCol, ofColor _noCol) {
	
	glDisable(GL_LIGHTING);
	ofEnableSmoothing();
	
	float offsetBasis = 40;
	
	float heightAns = answerFont.getLineHeight();
	float heightNum = numberFont.getLineHeight();
	
	float heightOffset = 25;
	float leftOffset = 10;
	
	float bottom = offsetBasis+15;	
	
	float leftYes = offsetBasis;
	
	ofPushMatrix();
	ofSetColor(255, 255, 255);	
	ofTranslate(leftYes+leftOffset, ofGetScreenHeight()-bottom-heightNum+heightOffset, 0);
	ofEnableSmoothing();	
	ofEnableAlphaBlending(); 
	//answerFont.drawString(yes, 0, 0);
	ofDisableAlphaBlending(); 
	ofPopMatrix();
	ofPushMatrix();
	ofSetColor(_yesCol.r, _yesCol.g, _yesCol.b);
	ofTranslate(leftYes, ofGetScreenHeight()-bottom, 0);
	numberFont.drawString(yesNum, 0, 0);
	ofPopMatrix();
	
	
	float rightOffset = numberFont.stringWidth(noNum)+offsetBasis;
	float leftNo = ofGetScreenWidth()-rightOffset;
	ofPushMatrix();
	ofSetColor(255, 255, 255);
	ofTranslate(leftNo+leftOffset, ofGetScreenHeight()-bottom-heightNum+heightOffset, 0);
	ofEnableSmoothing();	
	ofEnableAlphaBlending(); 
	//answerFont.drawString(no, 0, 0);
	ofDisableAlphaBlending(); 
	ofPopMatrix();
	ofPushMatrix();
	ofSetColor(_noCol.r, _noCol.g, _noCol.b);
	ofTranslate(leftNo, ofGetScreenHeight()-bottom, 0);
	numberFont.drawString(noNum, 0, 0);
	ofPopMatrix();

	glEnable(GL_LIGHTING);	

}

string StateText::insertComma(string num) {
	
	string rtn = num;
	int len = num.size();
	if (len > 3) {
		int commas = len/3;
		int times = 0;
		for (int i = 0; i < commas; i++) {
			rtn.insert(i*3+times+1, ",");
			times++;
		}
	}
	return rtn;
	
}

void StateText::updateColor(float colAngYes, float colAngNo) {
	
	ofColor thisYesCol;
	ofColor thisNoCol;
	
	if (colAngYes != 1000.0) {
		colp.setColorAngle(colAngYes);
		colp.setColorScale(1.0);
		colp.setColorRadius(1.0);
		colp.update();
		thisYesCol = colp.getColor();
	}else {
		thisYesCol.r = 255.0; thisYesCol.g = 255.0; thisYesCol.b = 255.0;
	}
	
	if (colAngNo != 1000.0) {
		colp.setColorAngle(colAngNo);
		colp.setColorScale(1.0);
		colp.setColorRadius(1.0);
		colp.update();
		thisNoCol = colp.getColor();
	}else {
		thisNoCol.r = 255.0; thisNoCol.g = 255.0; thisNoCol.b = 255.0;
	}	
	
	updateColor(thisYesCol, thisNoCol);	
	
}

void StateText::updateColor(ofColor yesCol, ofColor noCol) {
	
//	yesColTween.setParameters(colTweenEasing, ofxTween::easeIn, yesCol.r, 255.0, 2000, 0);
//	yesColTween.addValue(yesCol.g, 255.0);
//	yesColTween.addValue(yesCol.b, 255.0);
//	
//	noColTween.setParameters(colTweenEasing, ofxTween::easeIn, noCol.r, 255.0, 2000, 0);
//	noColTween.addValue(noCol.g, 255.0);
//	noColTween.addValue(noCol.b, 255.0);
	
	yesColTween.setParameters(colTweenEasing, ofxTween::easeIn, yesCol.r, yesCol.r, 0, 0);
	yesColTween.addValue(yesCol.g, yesCol.g);
	yesColTween.addValue(yesCol.b, yesCol.b);
	
	noColTween.setParameters(colTweenEasing, ofxTween::easeIn, noCol.r, noCol.r, 0, 0);
	noColTween.addValue(noCol.g, noCol.g);
	noColTween.addValue(noCol.b, noCol.b);	
	
	curYesCol = yesCol;
	curNoCol = noCol;
	
}

void StateText::startFadeToDefaultColorYes(float dur) {
	
	yesColTween.setParameters(colTweenEasing, ofxTween::easeIn, curYesCol.r, 255.0, dur, 0);
	yesColTween.addValue(curYesCol.g, 255.0);
	yesColTween.addValue(curYesCol.b, 255.0);	
	
}

void StateText::startFadeToDefaultColorNo(float dur) {
	
	noColTween.setParameters(colTweenEasing, ofxTween::easeIn, curNoCol.r, 255.0, dur, 0);
	noColTween.addValue(curNoCol.g, 255.0);
	noColTween.addValue(curNoCol.b, 255.0);	
	
}

void StateText::updateColorYes(float colAngYes) {
	
	ofColor thisYesCol;
	ofColor thisNoCol;
	
	if (colAngYes != 1000.0) {
		colp.setColorAngle(colAngYes);
		colp.setColorScale(1.0);
		colp.setColorRadius(1.0);
		colp.update();
		thisYesCol = colp.getColor();
	}else {
		thisYesCol.r = 255.0; thisYesCol.g = 255.0; thisYesCol.b = 255.0;
	}
	
	thisNoCol.r = 255.0; thisNoCol.g = 255.0; thisNoCol.b = 255.0;
	updateColor(thisYesCol, thisNoCol);

}

void StateText::updateColorNo(float colAngNo) {
	
	ofColor thisYesCol;
	ofColor thisNoCol;
	
	if (colAngNo != 1000.0) {
		colp.setColorAngle(colAngNo);
		colp.setColorScale(1.0);
		colp.setColorRadius(1.0);
		colp.update();
		thisNoCol = colp.getColor();
	}else {
		thisNoCol.r = 255.0; thisNoCol.g = 255.0; thisNoCol.b = 255.0;
	}	
	
	thisYesCol.r = 255.0; thisYesCol.g = 255.0; thisYesCol.b = 255.0;
	updateColor(thisYesCol, thisNoCol);	
	
}

void StateText::updateColorYes(ofColor _yesCol) {
	
	ofColor thisNoCol;
	thisNoCol.r = 255.0; thisNoCol.g = 255.0; thisNoCol.b = 255.0;
	updateColor(_yesCol, thisNoCol);
	
}

void StateText::updateColorNo(ofColor _noCol) {

	ofColor thisYesCol;
	thisYesCol.r = 255.0; thisYesCol.g = 255.0; thisYesCol.b = 255.0;
	updateColor(thisYesCol, _noCol);

}