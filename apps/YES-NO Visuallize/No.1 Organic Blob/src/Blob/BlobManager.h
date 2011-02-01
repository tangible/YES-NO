/*
 *  BlobManager.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxShader.h"
#include "ofxBullet.h"
#include "AdminPanel.h"
#include "Metaballs.h"
#include "Marchingcubes.h"

class BlobManager {

public:
	void setup(int _fps, AdminPanel* _admin);
	void update();
	void draw();
	
	void changeImg(string path);
	
	void setupForNoTexturing();
	void huntForBlendFunc (int period, int defaultSid, int defaultDid);	
	
	AdminPanel*				admin;
	
	int						fps;
	ofxShader				shader;
	ofVideoPlayer			player;
	bool					isVidTex;
	int						texSlot;
	ofImage					img;
	ofImage					bg;
	
	ofxBullet*				bullet;
	vector<MyRigidBody*>	spheres;		
	
	CMetaballs*				m_pMetaballs;
	ofPoint*				ballPoints;
	ofPoint*				ballPointsPrev;
	ofPoint*				ballPointsPrev2;
	float*					ballSizes;
	int*					ballLevels;
	int						nPoints;
	int						nMetaBalls;
	ofPoint					boundsAvg;
	float					boundsScaling;
	
	int						blurValue;
	int						counter;
	int						counter2;
	float					maxDeceleration;
	
	float*					screenDepth;   // the fetched depth buffer
	int						nScreenPixels;  // number of pixels in the display device
	int						screenW;        // width of the display device
	int						screenH;        // height of the display device
	ofTexture				shadowTex;      // texture object for displaying the shadow
	ofxCvGrayscaleImage		shadowCvImage;
	int						shadowDivX;     // ratio in X of screenW:shadowW
	int						shadowDivY;
	unsigned char*			shadowPixels;
	unsigned char*			shadowPixelsLA; // Luminance + Alpha
	int						nShadowPixels;
	int						shadowW;
	int						shadowH;
	
};