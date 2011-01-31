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

void AdminPanel::setup(){
	
	gui.addTitle("Graphical Setting");
	gui.addButton("change img", changeImgBtn);
	gui.addSlider("Shadow Intensity", SHADOWINTENSITY, 0.0, 1.0);
	gui.addSlider("Blob Tex&Col Mix Ratio", TEXCOLMIXRATIO, 0.0, 1.0);
	gui.addSlider("Blob Transparency", BLOBTRANSPARENCY, 0.0, 1.0);
	gui.addColorPicker("Blob Base Color", BLOBBASECOL).setNewColumn(true);
	gui.addColorPicker("Blob Material Ambient", BLOBMATERIALAMBIENT);
	gui.addColorPicker("Blob Material Diffuse", BLOBMATERIALDIFFUSE);
	gui.addColorPicker("Blob Material Specular", BLOBMATERIALSPECULAR);
	gui.addSlider("Blob Material Shineness", BLOBMATERIALSHINENESS, 0.0, 128.0);	
	gui.addColorPicker("Light Ambient", LIGHTAMBIENT).setNewColumn(true);
	gui.addColorPicker("Light Diffuse", LIGHTDIFFUSE);
	gui.addColorPicker("Light Specular", LIGHTSPECULAR);	
	
//	gui.loadFromXML();
	gui.show();
	
	// default init
	SHADOWINTENSITY = 0.5;
	TEXCOLMIXRATIO = 0.5;
	BLOBTRANSPARENCY = 0.5;
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
	BLOBMATERIALSHINENESS = 100;
	LIGHTAMBIENT[0] = 1.00;
	LIGHTAMBIENT[1] = 0.99;
	LIGHTAMBIENT[2] = 0.98;
	LIGHTAMBIENT[3] = 0.10;	
	LIGHTDIFFUSE[0] = 0.10;
	LIGHTDIFFUSE[1] = 0.10;
	LIGHTDIFFUSE[2] = 1.0;
	LIGHTDIFFUSE[3] = 1.0;	
	LIGHTSPECULAR[0] = 0.45;
	LIGHTSPECULAR[1] = 0.43;
	LIGHTSPECULAR[2] = 0.44;
	LIGHTSPECULAR[3] = 1.0;		
	
};

void AdminPanel::update(){
	
	if(changeImgBtn) {
		changeImgBtn = false;
		openFileDialogue("img");
	}	
	
};

void AdminPanel::draw(){
	
	glDisable(GL_LIGHTING);
	gui.draw();
	glEnable(GL_LIGHTING);

};

void AdminPanel::toggle(){

	gui.toggleDraw();
	if (gui.isOn()) {
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}else {
		glutSetCursor(GLUT_CURSOR_NONE);
	}	
	
};

void AdminPanel::openFileDialogue(string ID){
	
	string path;
	ofxFileDialogOSX::openFile(path);
	FileDef fi;
	fi.ID = ID;
	fi.path = path;
	ofNotifyEvent(onFileDialogue, fi);

};