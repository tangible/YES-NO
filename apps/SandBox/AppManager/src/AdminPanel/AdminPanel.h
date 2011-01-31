/*
 *  AdminPanel.h
 *  AppManager
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFileDialogOSX.h"

class FileDef {
public:
	string ID;
	string path;	
};

class AdminPanel {
	
public:
	void setup();
	void update();
	void draw();
	void toggle();
	void openFileDialogue(string ID);
	
	ofEvent<FileDef> onFileDialogue; 
	
};
