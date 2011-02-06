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
	void openFileDialogueBG(string ID);
	void openFileDialogueChangeQImg(string ID);
	void openFileDialogueBlobTex(string ID);	
	void restoreDefault();
	
	ofEvent<FileDef> onFileDialogueBG; 
	ofEvent<FileDef> onFileDialogueQImg;
	ofEvent<FileDef> onFileDialogueBlobTex; 	
	
	int		PHYSICSTICKFPS;
	bool	TOGGLEMOTION;
	bool	CHANGEBGBTN;
	bool	CHANGEQIMGBTN;
	bool	CHANGEBLOBTEXBTN;
	bool	RESTORDEFBTN;
	float	SHADOWINTENSITY;
	int		SHADOWSCALE;
	int		SHADOWPOSX;
	int		SHADOWPOSY;
	float	SHADOWROTDEG;
	bool	SHADOWROTX;
	bool	SHADOWROTY;
	bool	SHADOWROTZ;	
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
	
	int		TESTX;
	int		TESTY;
	bool	DRAWDEBUG;
	
};
