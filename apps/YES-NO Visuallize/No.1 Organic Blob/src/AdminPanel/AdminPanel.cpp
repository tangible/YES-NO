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
	
	gui.addTitle("Motion Setting");
	gui.addSlider("Motion Tick PerSec", PHYSICSTICKFPS, 10, 100);
	gui.addToggle("Toggle Motion", TOGGLEMOTION);
	gui.addTitle("Graphics Setting").setNewColumn(true);
	gui.addButton("Change Texture", CHANGETEXBTN);
	gui.addButton("Restore Default", RESTORDEFBTN);
	gui.addSlider("Shadow Intensity", SHADOWINTENSITY, 0.0, 1.0);
	gui.addSlider("Shadow Scale", SHADOWSCALE, 0, 20);
	gui.addSlider("Shadow Pos X", SHADOWPOSX, -1000, 1000);
	gui.addSlider("Shadow Pos Y", SHADOWPOSY, -1000, 1000);
	gui.addSlider("Shadow RotDegree", SHADOWROTDEG, 0.0, 360.0);
	gui.addToggle("Shadow Rot X", SHADOWROTX);
	gui.addToggle("Shadow Rot Y", SHADOWROTY);
	gui.addToggle("Shadow Rot Z", SHADOWROTZ);
	gui.addSlider("Light Pos X", LIGHTX, -1000.0, 1000.0).setNewColumn(true);
	gui.addSlider("Light Pos Y", LIGHTY, -1000.0, 1000.0);
	gui.addSlider("Light Pos Z", LIGHTZ, -1000.0, 1000.0);	
	gui.addSlider("Blob Tex&Col Mix Ratio", TEXCOLMIXRATIO, 0.0, 1.0);
	gui.addSlider("Blob Transparency", BLOBTRANSPARENCY, 0.0, 1.0);
	gui.addColorPicker("Blob Base Color", BLOBBASECOL).setNewColumn(true);
//	gui.addColorPicker("Blob Material Ambient", BLOBMATERIALAMBIENT);
//	gui.addColorPicker("Blob Material Diffuse", BLOBMATERIALDIFFUSE);
	gui.addColorPicker("Blob Material Specular", BLOBMATERIALSPECULAR);
	gui.addSlider("Blob Material Shineness", BLOBMATERIALSHINENESS, 0.0, 50.0);	
	gui.addColorPicker("Light Ambient", LIGHTAMBIENT);
	gui.addColorPicker("Light Diffuse", LIGHTDIFFUSE);
	gui.addColorPicker("Light Specular", LIGHTSPECULAR);	
	
	gui.addTitle("test param");
	gui.addSlider("test x", TESTX, -1000, 1000);
	gui.addSlider("test x", TESTY, -1000, 1000);	
	
//	gui.loadFromXML();
	gui.show();
	
	restoreDefault();
	
}

void AdminPanel::update(){
	
	if(CHANGETEXBTN) {
		CHANGETEXBTN = false;
		openFileDialogue("img");
	}else if (RESTORDEFBTN) {
		restoreDefault();
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

void AdminPanel::openFileDialogue(string ID){
	
	string path;
	ofxFileDialogOSX::openFile(path);
	FileDef fi;
	fi.ID = ID;
	fi.path = path;
	ofNotifyEvent(onFileDialogue, fi);

}

void AdminPanel::restoreDefault() {

	// default init
	PHYSICSTICKFPS = 25;
	TOGGLEMOTION = true;
	CHANGETEXBTN = false;
	RESTORDEFBTN = false;
	SHADOWINTENSITY = 0.658;
	SHADOWSCALE = 12;
	SHADOWPOSX = 0;
	SHADOWPOSY = 0;
	SHADOWROTDEG = 60.0;
	SHADOWROTX = true;
	SHADOWROTY = false;
	SHADOWROTZ = false;
	LIGHTX = 0.0;
	LIGHTY = -488.0;
	LIGHTZ = 895.0;
	TEXCOLMIXRATIO = 0.224;
	BLOBTRANSPARENCY = 0.8;
	BLOBBASECOL[0] = 0.5;
	BLOBBASECOL[1] = 0.5;
	BLOBBASECOL[2] = 0.5;
	BLOBBASECOL[3] = 1.0;	
	BLOBMATERIALAMBIENT[0] = 1.0;
	BLOBMATERIALAMBIENT[1] = 1.0;
	BLOBMATERIALAMBIENT[2] = 1.0;
	BLOBMATERIALAMBIENT[3] = 1.0;	
	BLOBMATERIALDIFFUSE[0] = 1.0;
	BLOBMATERIALDIFFUSE[1] = 1.0;
	BLOBMATERIALDIFFUSE[2] = 1.0;
	BLOBMATERIALDIFFUSE[3] = 1.0;	
	BLOBMATERIALSPECULAR[0] = 1.0;
	BLOBMATERIALSPECULAR[1] = 1.0;
	BLOBMATERIALSPECULAR[2] = 1.0;
	BLOBMATERIALSPECULAR[3] = 1.0;		
	BLOBMATERIALSHINENESS = 2.5;
	LIGHTAMBIENT[0] = 0.0645;
	LIGHTAMBIENT[1] = 0.0645;
	LIGHTAMBIENT[2] = 0.0645;
	LIGHTAMBIENT[3] = 1.0;	
	LIGHTDIFFUSE[0] = 0.566;
	LIGHTDIFFUSE[1] = 0.566;
	LIGHTDIFFUSE[2] = 0.566;
	LIGHTDIFFUSE[3] = 1.0;	
	LIGHTSPECULAR[0] = 0.94;
	LIGHTSPECULAR[1] = 0.94;
	LIGHTSPECULAR[2] = 0.94;
	LIGHTSPECULAR[3] = 1.0;	
	
	TESTX = 0;
	TESTY = 0;

}