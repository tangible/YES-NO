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
	
	gui.addTitle("A group");
	gui.addButton("change img", changeImgBtn);
	gui.loadFromXML();
	gui.show();
	
};

void AdminPanel::update(){
	
	if(changeImgBtn) {
		changeImgBtn = false;
		openFileDialogue("img");
	}	
	
};

void AdminPanel::draw(){
	
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
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