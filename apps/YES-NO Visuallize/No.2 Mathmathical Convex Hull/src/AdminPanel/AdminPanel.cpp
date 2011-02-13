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
	LIGHTY = 777.0;
	LIGHTZ = -746.0;	
	MATERIALAMBIENT[0] = 0.25;
	MATERIALAMBIENT[1] = 0.25;
	MATERIALAMBIENT[2] = 0.25;
	MATERIALAMBIENT[3] = 1.0;	
	MATERIALDIFFUSE[0] = 0.4;
	MATERIALDIFFUSE[1] = 0.4;
	MATERIALDIFFUSE[2] = 0.4;
	MATERIALDIFFUSE[3] = 1.0;	
	MATERIALSPECULAR[0] = 0.774597;
	MATERIALSPECULAR[1] = 0.774597;
	MATERIALSPECULAR[2] = 0.774597;
	MATERIALSPECULAR[3] = 1.0;		
	MATERIALSHINENESS = 0.6;
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
	
}