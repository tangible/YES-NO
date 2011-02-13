/*
 *  AdminPanel.h
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/10.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

class AdminPanel {
	
public:
	void setup();
	void update();
	void draw();
	void toggle();
	void restoreDefault();	

	bool	RESTORDEFBTN;
	float	LIGHTX;
	float	LIGHTY;
	float	LIGHTZ;
	float	MATERIALAMBIENT[4];
	float	MATERIALDIFFUSE[4];
	float	MATERIALSPECULAR[4];
	float	MATERIALSHINENESS;
	float	LIGHTAMBIENT[4];
	float	LIGHTDIFFUSE[4];
	float	LIGHTSPECULAR[4];	
	
	float	AMBIENTINTENSITY;
	float	DEFFUSEINTENSITY;
	float	SPECULARINTENSITY;
	float	ROUGHNESS;
	float	SHARPNESS;
	
};