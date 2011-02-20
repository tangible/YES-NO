/*
 *  AdminPanel.cpp
 *  No.3 Particle Cloud
 *
 *  Created by Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "AdminPanel.h"
#include "ofxSimpleGuiToo.h"

void AdminPanel::setup() {
	
	gui.addTitle("SSAO Setting");
	gui.addSlider("camerarangex", camerarangex, 0, 10000);
	gui.addSlider("camerarangey", camerarangey, 0, 10000);
	gui.addSlider("aoCap", aoCap, 0.0, 10.0);
	gui.addSlider("aoMultiplier", aoMultiplier, 0.0, 20000.0);
	gui.addSlider("depthTolerance", depthTolerance, 0.000, 0.002);
	gui.addSlider("aorange", aorange, 0.0, 2.0);
	gui.addSlider("readDepthVal", readDepthVal, 0.0, 20.0);
	gui.addSlider("aoDivision", aoDivision, 0, 30);
	gui.addSlider("baseColSubdivision", baseColSubdivision, 0.000, 1.000);
	
	gui.addTitle("DOF Setting").setNewColumn(true);
	gui.addSlider("focus", focus, 0.0, 2.0);
	gui.addSlider("aspectratiox", aspectratiox, 0.0, ofGetWidth());
	gui.addSlider("aspectratioy", aspectratioy, 0.0, ofGetHeight());
	gui.addSlider("blurclamp", blurclamp, 0.0, 1.0);
	gui.addSlider("bias", bias, 0.0, 1.0);	
	
	gui.addButton("Restore Default", RESTORDEFBTN).setNewColumn(true);
	gui.addToggle("Toggle Simu", TOGGLEMOTION);	
	gui.addSlider("colScale", colScale, 0.0, 1.0);
	gui.addSlider("colRadius", colRadius, 0.0, 1.0);
	gui.addSlider("colAngle", colAngle, -1.0, 1.0);
	gui.addSlider("sizeBase", sizeBase, -100, 100);
	gui.loadFromXML();
	gui.show();	
	
	restoreDefault();
	
}

void AdminPanel::update(){
	
	if (RESTORDEFBTN) {
		restoreDefault();
	}
	
}

void AdminPanel::draw(){
	
	glDisable(GL_LIGHTING);
	ofSetupScreen();
	gui.draw();
	
}

void AdminPanel::toggle(){
	
	gui.toggleDraw();

}

void AdminPanel::restoreDefault() {

	camerarangex = 6113.28;
	camerarangey = 4121.09;
	aoCap = 1.9995;
	aoMultiplier = 351.562;
	depthTolerance = 0.0004730;
	aorange = 0.385156;
	readDepthVal = 18.226;
	aoDivision = 12.5;
	baseColSubdivision = 0.896;
	
	focus = 1.028594;
	aspectratiox = ofGetWidth();
	aspectratioy = ofGetHeight();	
	blurclamp = 0.0253910;
	bias = 0.041016;
	
	colScale = 1.0;
	colRadius = 0.5;
	colAngle = 0.5;
	sizeBase = -6.0;
	
	RESTORDEFBTN = false;
	TOGGLEMOTION = true;

}