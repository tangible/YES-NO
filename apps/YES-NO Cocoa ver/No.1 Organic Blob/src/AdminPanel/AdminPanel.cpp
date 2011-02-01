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

bool changeImgBtn = false;
bool restoreDefBtn = false;

void AdminPanel::setup(){
	
	gui.addTitle("Motion Setting");
	gui.addSlider("Motion Tick FPS", PHYSICSTICKFPS, 10, 100);
	gui.addTitle("Graphics Setting");
	gui.addButton("Change Texture", changeImgBtn);
	gui.addButton("Restore Default", restoreDefBtn);
	gui.addSlider("Shadow Intensity", SHADOWINTENSITY, 0.0, 1.0);
	gui.addSlider("Light Pos X", LIGHTX, -1000.0, 1000.0);
	gui.addSlider("Light Pos Y", LIGHTY, -1000.0, 1000.0);
	gui.addSlider("Light Pos Z", LIGHTZ, -1000.0, 1000.0);	
	gui.addSlider("Blob Tex&Col Mix Ratio", TEXCOLMIXRATIO, 0.0, 1.0);
	gui.addSlider("Blob Transparency", BLOBTRANSPARENCY, 0.0, 1.0);
	gui.addColorPicker("Blob Base Color", BLOBBASECOL).setNewColumn(true);
	gui.addColorPicker("Blob Material Ambient", BLOBMATERIALAMBIENT);
	gui.addColorPicker("Blob Material Diffuse", BLOBMATERIALDIFFUSE);
	gui.addColorPicker("Blob Material Specular", BLOBMATERIALSPECULAR);
	gui.addSlider("Blob Material Shineness", BLOBMATERIALSHINENESS, 0.0, 50.0);	
	gui.addColorPicker("Light Ambient", LIGHTAMBIENT).setNewColumn(true);
	gui.addColorPicker("Light Diffuse", LIGHTDIFFUSE);
	gui.addColorPicker("Light Specular", LIGHTSPECULAR);	
	
//	gui.loadFromXML();
	gui.show();
	
	restoreDefault();
	
}

void AdminPanel::update(){
	
	if(changeImgBtn) {
		changeImgBtn = false;
		openFileDialogue("img");
	}else if (restoreDefBtn) {
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
//	if (gui.isOn()) {
//		glutSetCursor(GLUT_CURSOR_INHERIT);
//	}else {
//		glutSetCursor(GLUT_CURSOR_NONE);
//	}	
	
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
	SHADOWINTENSITY = 0.658;
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

}