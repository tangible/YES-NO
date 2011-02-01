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
	void restoreDefault();
	
	ofEvent<FileDef> onFileDialogue; 
	
	int		PHYSICSTICKFPS;
	float	SHADOWINTENSITY;
	float	LIGHTX;
	float	LIGHTY;
	float	LIGHTZ;
	float	TEXCOLMIXRATIO;
	float	BLOBTRANSPARENCY;
	float	BLOBBASECOL[4];
	float	BLOBMATERIALAMBIENT[4];
	float	BLOBMATERIALDIFFUSE[4];
	float	BLOBMATERIALSPECULAR[4];
	float	BLOBMATERIALSHINENESS;
	float	LIGHTAMBIENT[4];
	float	LIGHTDIFFUSE[4];
	float	LIGHTSPECULAR[4];	
	
};
