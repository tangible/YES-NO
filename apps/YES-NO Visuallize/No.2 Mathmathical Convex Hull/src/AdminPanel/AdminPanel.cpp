/*
 *  AdminPanel.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/10.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "AdminPanel.h"
#include "ofxSimpleGuiToo.h"

void AdminPanel::setup(){
	
	gui.addTitle("Image Setting");
	gui.addButton("Change BG", changeBG);
	gui.addButton("Clear BG", clearBG);
	gui.addButton("Change Question Img", changeQImg);
	gui.addButton("Clear Question Img", clearQImg);		
//	gui.addColorPicker("Obj Edge Color", BGColor);	
	
	gui.addTitle("SMS Setting").setNewColumn(true);
	gui.addButton("load setting", loadSetting);	
	gui.addToggle("debug with fake SMS", debugWithFakeSMS);
	gui.addButton("get all SMS (1time & irreversible!)", restoreAllSMSAnswer);
	
	gui.addTitle("Graphic Setting").setNewColumn(true);
	gui.addSlider("Light Pos X", LIGHTX, -2000.0, 2000.0);
	gui.addSlider("Light Pos Y", LIGHTY, -2000.0, 2000.0);
	gui.addSlider("Light Pos Z", LIGHTZ, -2000.0, 2000.0);		
	gui.addColorPicker("Material Ambient", MATERIALAMBIENT);
	gui.addColorPicker("Material Diffuse", MATERIALDIFFUSE);
	gui.addColorPicker("Material Specular", MATERIALSPECULAR);
	gui.addSlider("Material Shineness", MATERIALSHINENESS, 0.0, 50.0);	
	gui.addColorPicker("Light Ambient", LIGHTAMBIENT).setNewColumn(true);
	gui.addColorPicker("Light Diffuse", LIGHTDIFFUSE);
	gui.addColorPicker("Light Specular", LIGHTSPECULAR);	

	gui.addSlider("AmbientIntensity", AMBIENTINTENSITY, 0.0, 100.0);
	gui.addSlider("DiffuseIntensity", DEFFUSEINTENSITY, 0.0, 100.0);
	gui.addSlider("SpecularIntensity", SPECULARINTENSITY, 0.0, 100.0);
	gui.addSlider("Roughness", ROUGHNESS, 0.0, 1.0);
	gui.addSlider("Sharpness", SHARPNESS, 0.0, 1.0);
		
	gui.addButton("Restore Default", RESTORDEFBTN).setNewColumn(true);

	
	gui.loadFromXML();
//	gui.show();	
	
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
		mySetting.removeTag("bg");		
		int i = 1;
		ofNotifyEvent(onClearBG, i);
	}else if (changeQImg) {
		changeQImg = false;
		openFileDialogueChangeQImg("QImg");
	}else if (clearQImg) {
		clearQImg = false;
		mySetting.removeTag("qimg");		
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
	glEnable(GL_LIGHTING);
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
	
	mySetting.saveToXMLFile("bg", path);
	
}

void AdminPanel::openFileDialogueChangeQImg(string ID) {
	
	string path;
	ofxFileDialogOSX::openFile(path);
	FileDef fi;
	fi.ID = ID;
	fi.path = path;
	ofNotifyEvent(onFileDialogueQImg, fi);	
	
	mySetting.saveToXMLFile("qimg", path);
	
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
	
	mySetting.saveToXMLFile("phonenumber", phone_questionID);
	mySetting.saveToXMLFile("from", kioskPhoneNum_asFrom);
	cout << "set to AppSpecificSetting.xml" << endl;
	
}

bool AdminPanel::checkSetting() {
	
	cout << "checking AppSpecificSetting.xml....";
	
	bool bPhonnumber = mySetting.tagExists("phonenumber");
	bool bFrom = mySetting.tagExists("from");
	
	if (bPhonnumber) {
		phone_questionID = mySetting.getValueByTag("phonenumber", "error");
		cout << "phone_questionID = "+phone_questionID;
	}
	if (bFrom) {
		kioskPhoneNum_asFrom = mySetting.getValueByTag("from", "error");		
		cout << " kioskPhoneNum_asFrom = "+kioskPhoneNum_asFrom;
	}
	
	bool bg = mySetting.tagExists("bg");
	bool qimg = mySetting.tagExists("qimg");
	if (bg) {
		string bgpath = mySetting.getValueByTag("bg", "error");
		FileDef fi;
		fi.ID = "bg";
		fi.path = bgpath;
		ofNotifyEvent(onFileDialogueBG, fi);
	}
	if (qimg) {
		string qpath = mySetting.getValueByTag("qimg", "error");
		FileDef fi;
		fi.ID = "qimg";
		fi.path = qpath;
		ofNotifyEvent(onFileDialogueQImg, fi);		
	}
	
	if ((phone_questionID != "error" && kioskPhoneNum_asFrom != "error") &&
		(bPhonnumber && bFrom)) {
		cout << " true!" << endl;
		return true;
	}else {
		cout << " false!" << endl;		
		return false;
	}
	
}

void AdminPanel::restoreDefault() {

	RESTORDEFBTN = false;
	LIGHTX = 0.0;
	LIGHTY = 1343.0;
	LIGHTZ = -1346.0;	
	MATERIALAMBIENT[0] = 0.61;
	MATERIALAMBIENT[1] = 0.61;
	MATERIALAMBIENT[2] = 0.61;
	MATERIALAMBIENT[3] = 1.0;	
	MATERIALDIFFUSE[0] = 0.4;
	MATERIALDIFFUSE[1] = 0.4;
	MATERIALDIFFUSE[2] = 0.4;
	MATERIALDIFFUSE[3] = 1.0;	
	MATERIALSPECULAR[0] = 0.594597;
	MATERIALSPECULAR[1] = 0.594597;
	MATERIALSPECULAR[2] = 0.594597;
	MATERIALSPECULAR[3] = 1.0;		
	MATERIALSHINENESS = 0.6;
	LIGHTAMBIENT[0] = 1.0;
	LIGHTAMBIENT[1] = 1.0;
	LIGHTAMBIENT[2] = 1.0;
	LIGHTAMBIENT[3] = 1.0;	
	LIGHTDIFFUSE[0] = 1.0;
	LIGHTDIFFUSE[1] = 1.0;
	LIGHTDIFFUSE[2] = 1.0;
	LIGHTDIFFUSE[3] = 1.0;	
	LIGHTSPECULAR[0] = 0.46;
	LIGHTSPECULAR[1] = 0.46;
	LIGHTSPECULAR[2] = 0.46;
	LIGHTSPECULAR[3] = 1.0;		
	
	changeBG = false;
	clearBG = false;
	BGColor[0] = 0.0;
	BGColor[1] = 0.0;
	BGColor[2] = 0.0;
	BGColor[3] = 1.0;	
	changeQImg = false;
	clearQImg = false;	
	
	debugWithFakeSMS = false;
	restoreAllSMSAnswer = false;
	loadSetting = false;	
	
	phone_questionID = "";
	kioskPhoneNum_asFrom = "";	
}