/*
 *  AdminPanel.cpp
 *  AppManager
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "AdminPanel.h"
#include "ofxSimpleGuiToo.h"

void AdminPanel::setup(){
	
//	gui.addTitle("Motion Setting");
//	gui.addSlider("Motion Tick PerSec", PHYSICSTICKFPS, 10, 100);
//	gui.addToggle("Toggle Motion", TOGGLEMOTION);

	gui.addTitle("Image Setting");
	gui.addButton("Change BG", CHANGEBGBTN);
	gui.addButton("Clear BG", clearBG);	
	gui.addButton("Change Question Img", CHANGEQIMGBTN);
	gui.addButton("Clear Question Img", clearQImg);			
//	gui.addButton("Change Blob Tex", CHANGEBLOBTEXBTN);
	
	gui.addTitle("SMS Setting").setNewColumn(true);
	gui.addButton("load setting", loadSetting);
	gui.addToggle("debug with fake SMS", debugWithFakeSMS);
	gui.addButton("get all SMS (1time & irreversible!)", restoreAllSMSAnswer);
	
	gui.addTitle("Graphic Setting").setNewColumn(true);	
	gui.addSlider("Shadow Intensity", SHADOWINTENSITY, 0.0, 1.0);
	gui.addSlider("Shadow Scale", SHADOWSCALE, 0, 20);
	gui.addSlider("Shadow Pos X", SHADOWPOSX, -1000, 1000);
	gui.addSlider("Shadow Pos Y", SHADOWPOSY, -1000, 1000);
	gui.addSlider("Shadow RotDegree", SHADOWROTDEG, 0.0, 360.0);
	gui.addToggle("Shadow Rot X", SHADOWROTX);
	gui.addToggle("Shadow Rot Y", SHADOWROTY);
	gui.addToggle("Shadow Rot Z", SHADOWROTZ);
	gui.addSlider("Light Pos X", LIGHTX, -1000.0, 1000.0);
	gui.addSlider("Light Pos Y", LIGHTY, -1000.0, 1000.0);
	gui.addSlider("Light Pos Z", LIGHTZ, -1000.0, 1000.0);	
	gui.addSlider("Blob Tex&Col Mix Ratio", TEXCOLMIXRATIO, 0.0, 1.0);
	gui.addSlider("Blob Transparency", BLOBTRANSPARENCY, 0.0, 1.0);
	gui.addColorPicker("Blob Base Color", BLOBBASECOL);
//	gui.addColorPicker("Blob Material Ambient", BLOBMATERIALAMBIENT);
//	gui.addColorPicker("Blob Material Diffuse", BLOBMATERIALDIFFUSE);
	gui.addColorPicker("Blob Material Specular", BLOBMATERIALSPECULAR);
	gui.addSlider("Blob Material Shineness", BLOBMATERIALSHINENESS, 0.0, 50.0);	
	gui.addColorPicker("Light Ambient", LIGHTAMBIENT);
	gui.addColorPicker("Light Diffuse", LIGHTDIFFUSE);
	gui.addColorPicker("Light Specular", LIGHTSPECULAR);	
	
	gui.addButton("Restore Default", RESTORDEFBTN);
	
//	gui.addTitle("Debug Param");
//	gui.addToggle("DRAWDEBUG", DRAWDEBUG);
	
//	gui.loadFromXML();
//	gui.show();
	
	restoreDefault();
	
}

void AdminPanel::update(){
	
	if(CHANGEBGBTN) {
		CHANGEBGBTN = false;
		openFileDialogueBG("BG");
	}else if (CHANGEQIMGBTN) {
		CHANGEQIMGBTN = false;
		openFileDialogueChangeQImg("qimg");
	}else if (CHANGEBLOBTEXBTN) {
		CHANGEBLOBTEXBTN = false;
		openFileDialogueBlobTex("BlobTex");
	}else if (RESTORDEFBTN) {
		restoreDefault();
	}else if (clearBG) {
		clearBG = false;
		int i = 1;
		ofNotifyEvent(onClearBG, i);
	}else if (clearQImg) {
		clearQImg = false;
		int i = 1;
		ofNotifyEvent(onClearQImg, i);
	}else if (restoreAllSMSAnswer) {
		restoreAllSMSAnswer = false;
		int i = 1;
		ofNotifyEvent(onRestoreAllSMSAnswer, i);
	}else if (loadSetting) {
		loadSetting = false;
		openFileDialogueSetting("setting");
	}
	
}

void AdminPanel::draw(){
	
	glDisable(GL_LIGHTING);
	gui.draw();
	glEnable(GL_LIGHTING);

}

void AdminPanel::toggle(){

	gui.toggleDraw();
	if (gui.isOn()) {
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}else {
		glutSetCursor(GLUT_CURSOR_NONE);
	}	
	
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

void AdminPanel::openFileDialogueBG(string ID){
	
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

void AdminPanel::openFileDialogueBlobTex(string ID){
	
	string path;
	ofxFileDialogOSX::openFile(path);
	FileDef fi;
	fi.ID = ID;
	fi.path = path;
	ofNotifyEvent(onFileDialogueBlobTex, fi);
	
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

	// default init
	PHYSICSTICKFPS = 25;
	TOGGLEMOTION = true;
	CHANGEBGBTN = false;
	clearBG = false;
	CHANGEQIMGBTN = false;
	clearQImg = false;	
	CHANGEBLOBTEXBTN = false;
	RESTORDEFBTN = false;
	SHADOWINTENSITY = 0.88;
	SHADOWSCALE = 10;
	SHADOWPOSX = -140;
	SHADOWPOSY = -140;
	SHADOWROTDEG = 60.0;
	SHADOWROTX = false;
	SHADOWROTY = false;
	SHADOWROTZ = false;
	LIGHTX = 0.0;
	LIGHTY = 628.0;
	LIGHTZ = -480.0;
	TEXCOLMIXRATIO = 0.13;
	BLOBTRANSPARENCY = 0.83;
	BLOBBASECOL[0] = 0.5;
	BLOBBASECOL[1] = 0.5;
	BLOBBASECOL[2] = 0.5;
	BLOBBASECOL[3] = 1.0;	
	BLOBMATERIALAMBIENT[0] = 0.05;
	BLOBMATERIALAMBIENT[1] = 0.05;
	BLOBMATERIALAMBIENT[2] = 0.05;
	BLOBMATERIALAMBIENT[3] = 1.0;	
	BLOBMATERIALDIFFUSE[0] = 0.5;
	BLOBMATERIALDIFFUSE[1] = 0.5;
	BLOBMATERIALDIFFUSE[2] = 0.5;
	BLOBMATERIALDIFFUSE[3] = 1.0;	
	BLOBMATERIALSPECULAR[0] = 0.7;
	BLOBMATERIALSPECULAR[1] = 0.7;
	BLOBMATERIALSPECULAR[2] = 0.7;
	BLOBMATERIALSPECULAR[3] = 1.0;		
	BLOBMATERIALSHINENESS = 2.0;
	LIGHTAMBIENT[0] = 0.2;
	LIGHTAMBIENT[1] = 0.2;
	LIGHTAMBIENT[2] = 0.2;
	LIGHTAMBIENT[3] = 1.0;	
	LIGHTDIFFUSE[0] = 1.0;
	LIGHTDIFFUSE[1] = 1.0;
	LIGHTDIFFUSE[2] = 1.0;
	LIGHTDIFFUSE[3] = 1.0;	
	LIGHTSPECULAR[0] = 1.0;
	LIGHTSPECULAR[1] = 1.0;
	LIGHTSPECULAR[2] = 1.0;
	LIGHTSPECULAR[3] = 1.0;	
	
	DRAWDEBUG = false;
	
	debugWithFakeSMS = false;
	restoreAllSMSAnswer = false;	
	loadSetting = false;

}