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
	
	gui.addTitle("SMS Setting");
	gui.addButton("load setting", loadSetting);
	gui.addToggle("debug with fake SMS", debugWithFakeSMS);
	gui.addButton("get all SMS (1time & irreversible!)", restoreAllSMSAnswer);	
	
//	gui.addTitle("SSAO Setting").setNewColumn(true);
//	gui.addSlider("aoCap", aoCap, 0.0, 2.0);	
//	gui.addSlider("camerarangex", camerarangex, 0, 10000);
//	gui.addSlider("camerarangey", camerarangey, 0, 10000);
//	gui.addSlider("aoMultiplier", aoMultiplier, 0.0, 20000.0);
//	gui.addSlider("depthTolerance", depthTolerance, 0.000, 0.002);
//	gui.addSlider("aorange", aorange, 0.0, 2.0);
//	gui.addSlider("readDepthVal", readDepthVal, 0.0, 20.0);
//	gui.addSlider("aoDivision", aoDivision, 0, 30);
//	gui.addSlider("baseColSubdivision", baseColSubdivision, 0.000, 1.000);
	
//	gui.addTitle("DoF Setting");
//	gui.addSlider("bias", bias, 0.0, 1.0);	
//	gui.addSlider("focus", focus, 0.0, 2.0);
//	gui.addSlider("aspectratiox", aspectratiox, 0.0, ofGetScreenWidth());
//	gui.addSlider("aspectratioy", aspectratioy, 0.0, ofGetScreenHeight());
//	gui.addSlider("blurclamp", blurclamp, 0.0, 1.0);	
	
//	gui.addTitle("OBJ Setting").setNewColumn(true);
//	gui.addToggle("Toggle Simu", TOGGLEMOTION);	
//	gui.addSlider("colScale", colScale, 0.0, 1.0);
//	gui.addSlider("colRadius", colRadius, 0.0, 1.0);
//	gui.addSlider("colAngle", colAngle, -1.0, 1.0);
//	gui.addSlider("sizeBase", sizeBase, -100, 100);
	
	gui.addTitle("Graphic Setting").setNewColumn(true);
	gui.addSlider("Light Pos X", LIGHTX, -2000.0, 2000.0);
	gui.addSlider("Light Pos Y", LIGHTY, -2000.0, 2000.0);
	gui.addSlider("Light Pos Z", LIGHTZ, -2000.0, 2000.0);		
	gui.addColorPicker("Material Ambient", MATERIALAMBIENT);
	gui.addColorPicker("Material Diffuse", MATERIALDIFFUSE);
	gui.addColorPicker("Material Specular", MATERIALSPECULAR);
	gui.addSlider("Material Shineness", MATERIALSHINENESS, 0.0, 50.0);	
	gui.addColorPicker("Light Ambient", LIGHTAMBIENT);
	gui.addColorPicker("Light Diffuse", LIGHTDIFFUSE);
	gui.addColorPicker("Light Specular", LIGHTSPECULAR);	
	gui.addSlider("AmbientIntensity", AMBIENTINTENSITY, 0.0, 100.0);
	gui.addSlider("DiffuseIntensity", DEFFUSEINTENSITY, 0.0, 100.0);
	gui.addSlider("SpecularIntensity", SPECULARINTENSITY, 0.0, 100.0);	
	
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
	}else if (restoreAllSMSAnswer && (phone_questionID == "" || kioskPhoneNum_asFrom == "")) {
		loadSetting = false;
		openFileDialogueSetting("setting");	
		restoreAllSMSAnswer = false;
		int i = 1;
		ofNotifyEvent(onRestoreAllSMSAnswer, i);	
	}else if (restoreAllSMSAnswer) {
		restoreAllSMSAnswer = false;
		int i = 1;
		ofNotifyEvent(onRestoreAllSMSAnswer, i);
	}else if (loadSetting) {
		loadSetting = false;
		openFileDialogueSetting("setting");
	}else if (debugWithFakeSMS && (phone_questionID == "" || kioskPhoneNum_asFrom == "")) {
		loadSetting = false;
		openFileDialogueSetting("setting");		
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

void AdminPanel::openFileDialogueSetting(string ID) {
	
	string path;
	ofxFileDialogOSX::openFile(path);
	settingXML.loadFile(path);
	settingXML.pushTag("setting");
	phone_questionID = settingXML.getValue("phonenumber", "16048005302");
	kioskPhoneNum_asFrom = settingXML.getValue("from", "kiosk_1");
	cout << "phone_questionID = "+phone_questionID << endl;
	cout << "kioskPhoneNum_asFrom = "+kioskPhoneNum_asFrom << endl;
	settingXML.popTag();
	
}

void AdminPanel::restoreDefault() {

	aoCap = 0.02;	
	camerarangex = 6113.28;
	camerarangey = 4121.09;
	aoMultiplier = 351.562;
	depthTolerance = 0.0004730;
	aorange = 0.385156;
	readDepthVal = 18.226;
	aoDivision = 12.5;
	baseColSubdivision = 0.896;
	
	bias = 0.087;	
	focus = 0.8242;
	aspectratiox = ofGetScreenWidth();
	aspectratioy = ofGetHeight();	
	blurclamp = 0.0253910;
	
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
	
	LIGHTX = 0.0;
	LIGHTY = -625.0;
	LIGHTZ = 746.0;	
	MATERIALAMBIENT[0] = 0.95;
	MATERIALAMBIENT[1] = 0.95;
	MATERIALAMBIENT[2] = 0.95;
	MATERIALAMBIENT[3] = 1.0;	
	MATERIALDIFFUSE[0] = 0.95;
	MATERIALDIFFUSE[1] = 0.95;
	MATERIALDIFFUSE[2] = 0.95;
	MATERIALDIFFUSE[3] = 1.0;	
	MATERIALSPECULAR[0] = 0.83;
	MATERIALSPECULAR[1] = 0.83;
	MATERIALSPECULAR[2] = 0.83;
	MATERIALSPECULAR[3] = 1.0;		
	MATERIALSHINENESS = 24.0;
	LIGHTAMBIENT[0] = 1.0;
	LIGHTAMBIENT[1] = 1.0;
	LIGHTAMBIENT[2] = 1.0;
	LIGHTAMBIENT[3] = 1.0;	
	LIGHTDIFFUSE[0] = 1.0;
	LIGHTDIFFUSE[1] = 1.0;
	LIGHTDIFFUSE[2] = 1.0;
	LIGHTDIFFUSE[3] = 1.0;	
	LIGHTSPECULAR[0] = 0.54;
	LIGHTSPECULAR[1] = 0.54;
	LIGHTSPECULAR[2] = 0.54;
	LIGHTSPECULAR[3] = 1.0;		
	
	debugWithFakeSMS = false;
	restoreAllSMSAnswer = false;	
	loadSetting = false;

	phone_questionID = "";
	kioskPhoneNum_asFrom = "";	
}
