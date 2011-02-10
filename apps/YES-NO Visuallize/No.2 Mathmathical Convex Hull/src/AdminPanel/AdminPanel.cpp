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
	
	gui.addTitle("Setting");
	gui.addButton("Restore Default", RESTORDEFBTN);
	gui.addSlider("Light Pos X", LIGHTX, -1000.0, 1000.0);
	gui.addSlider("Light Pos Y", LIGHTY, -1000.0, 1000.0);
	gui.addSlider("Light Pos Z", LIGHTZ, -1000.0, 1000.0);		
	
	gui.loadFromXML();
//	gui.show();	
	
	restoreDefault();	
	
}

void AdminPanel::update(){
	
	if (RESTORDEFBTN) {
		restoreDefault();
	}
	
}

void AdminPanel::draw(){
	
	glDisable(GL_LIGHTING);
	gui.draw();
	//glEnable(GL_LIGHTING);
	
}

void AdminPanel::toggle(){
	
	gui.toggleDraw();
//	if (gui.isOn()) {
//		glutSetCursor(GLUT_CURSOR_INHERIT);
//	}else {
//		glutSetCursor(GLUT_CURSOR_NONE);
//	}	
//	
}

void AdminPanel::restoreDefault() {

	RESTORDEFBTN = false;
	LIGHTX = 0.0;
	LIGHTY = -10.0;
	LIGHTZ = -379.0;	
	
}