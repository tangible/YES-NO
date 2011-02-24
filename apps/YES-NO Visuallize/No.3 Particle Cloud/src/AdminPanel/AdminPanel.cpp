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
	
	gui.addTitle("Img Setting");
	gui.addButton("Change BG", changeBG);
	gui.addButton("Clear BG", clearBG);
	gui.addColorPicker("Obj Edge Color", BGColor);
	gui.addButton("Change Question Img", changeQImg);
	gui.addButton("Clear Question Img", clearQImg);	
	
	gui.addTitle("SSAO Setting").setNewColumn(true);
	gui.addSlider("aoCap", aoCap, 0.0, 2.0);	
	gui.addSlider("camerarangex", camerarangex, 0, 10000);
	gui.addSlider("camerarangey", camerarangey, 0, 10000);
	gui.addSlider("aoMultiplier", aoMultiplier, 0.0, 20000.0);
	gui.addSlider("depthTolerance", depthTolerance, 0.000, 0.002);
	gui.addSlider("aorange", aorange, 0.0, 2.0);
	gui.addSlider("readDepthVal", readDepthVal, 0.0, 20.0);
	gui.addSlider("aoDivision", aoDivision, 0, 30);
	gui.addSlider("baseColSubdivision", baseColSubdivision, 0.000, 1.000);
	
	gui.addTitle("DoF Setting").setNewColumn(true);
	gui.addSlider("bias", bias, 0.0, 1.0);	
	gui.addSlider("focus", focus, 0.0, 2.0);
//	gui.addSlider("aspectratiox", aspectratiox, 0.0, ofGetWidth());
//	gui.addSlider("aspectratioy", aspectratioy, 0.0, ofGetHeight());
	gui.addSlider("aspectratiox", aspectratiox, 0.0, ofGetScreenWidth());
	gui.addSlider("aspectratioy", aspectratioy, 0.0, ofGetScreenHeight());	
	gui.addSlider("blurclamp", blurclamp, 0.0, 1.0);	
	
	gui.addTitle("OBJ Setting").setNewColumn(true);
	gui.addToggle("Toggle Simu", TOGGLEMOTION);	
	gui.addSlider("colScale", colScale, 0.0, 1.0);
	gui.addSlider("colRadius", colRadius, 0.0, 1.0);
	gui.addSlider("colAngle", colAngle, -1.0, 1.0);
	gui.addSlider("sizeBase", sizeBase, -100, 100);
	
	
	gui.addButton("Restore Default", RESTORDEFBTN).setNewColumn(true);	
	
	gui.loadFromXML();
	//gui.show();	
	
	restoreDefault();
	
}

void AdminPanel::update(){
	
	if (RESTORDEFBTN) {
		restoreDefault();
	}else if (changeBG) {
		changeBG = false;
		openFileDialogueBG("BG");
	}else if (clearBG) {
		clearBG = false;
		int i = 1;
		ofNotifyEvent(onClearBG, i);
	}else if (changeQImg) {
		changeQImg = false;
		openFileDialogueChangeQImg("QImg");
	}else if (clearQImg) {
		clearQImg = false;
		int i = 1;
		ofNotifyEvent(onClearQImg, i);
	}
	
}

void AdminPanel::draw(){
	
	glDisable(GL_LIGHTING);
	ofSetupScreen();
	gui.draw();
	
}

void AdminPanel::keyPressed(int key){
	
	if(key>='0' && key<='9') {
		gui.setPage(key - '0');
		gui.show();
	} else {
		switch(key) {
			case ' ': gui.toggleDraw(); break;
			case '[': gui.prevPage(); break;
			case ']': gui.nextPage(); break;
			case 'p': gui.nextPageWithBlank(); break;
		}
	}	

}

void AdminPanel::openFileDialogueBG(string ID) {
	
	string path;
	ofxFileDialogOSX::openFile(path);
	FileDef fi;
	fi.ID = ID;
	fi.path = path;
	ofNotifyEvent(onFileDialogueBG, fi);
	
}

void AdminPanel::openFileDialogueChangeQImg(string ID) {
	
	string path;
	ofxFileDialogOSX::openFile(path);
	FileDef fi;
	fi.ID = ID;
	fi.path = path;
	ofNotifyEvent(onFileDialogueQImg, fi);	
	
}

void AdminPanel::restoreDefault() {

	camerarangex = 6113.28;
	camerarangey = 4121.09;
	aoCap = 0.304;
	aoMultiplier = 351.562;
	depthTolerance = 0.0004730;
	aorange = 0.385156;
	readDepthVal = 18.226;
	aoDivision = 12.5;
	baseColSubdivision = 0.896;
	
	focus = 0.761;
//	aspectratiox = ofGetWidth();
//	aspectratioy = ofGetHeight();	
	aspectratiox = ofGetScreenWidth();
	aspectratioy = ofGetScreenHeight();		
	blurclamp = 0.0253910;
	bias = 0.0019;
	
	colScale = 1.0;
	colRadius = 0.746;
	colAngle = 0.5;
	sizeBase = -6.0;
	
	RESTORDEFBTN = false;
	TOGGLEMOTION = true;
	
	changeBG = false;
	clearBG = false;
	BGColor[0] = 0.0;
	BGColor[1] = 0.0;
	BGColor[2] = 0.0;
	BGColor[3] = 1.0;	
	changeQImg = false;
	clearQImg = false;

}
