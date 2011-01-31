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
#include "Metaballs.h"
#include "Marchingcubes.h"
#include <stack>
#include "ofxBullet.h"
#include "AdminPanel.h"

class BlobManager {

public:
	void setup(int _fps, AdminPanel* _admin);
	void update();
	void draw();
	
	void changeImg(string path);
	
	void setupForTexturing();
	void setupForNoTexturing();
	void huntForBlendFunc (int period, int defaultSid, int defaultDid);	
	
	AdminPanel* admin;
	
	//---------------------------------------
	int				fps;
	ofxShader		shader;
	ofVideoPlayer	player;
	bool			isVidTex;
	int				texSlot;
	ofImage			img;
	ofImage			bg;
	
	ofxBullet*				bullet;
	vector<MyRigidBody*>	spheres;		
	
	//---------------------------------------
	int 	counter;
	int     counter2;
	
	//---------------------------------------
	CMetaballs  *m_pMetaballs;
	ofPoint     *ballPoints;
	ofPoint     *ballPointsPrev;
	ofPoint     *ballPointsPrev2;
	float       *ballMasses;
	int         *ballLevels;
	int         nPoints;
	int         nMetaBalls;
	
	float       maxDeceleration;
	float       decelerationThreshPct;
	int         decelerationEventCount;
	
	
	//---------------------------------------
	
	ofPoint     boundsMin;
	ofPoint     boundsMax;
	ofPoint     boundsAvg;
	float       boundsScaling;
	int         blurValue;
	float       motionBlur;
	
	// for shadow
	float       *screenDepth;   // the fetched depth buffer
	int         nScreenPixels;  // number of pixels in the display device
	int         screenW;        // width of the display device
	int         screenH;        // height of the display device
	ofTexture	shadowTex;      // texture object for displaying the shadow
	ofxCvGrayscaleImage  shadowCvImage;
	int         shadowDivX;     // ratio in X of screenW:shadowW
	int         shadowDivY;
	unsigned char *shadowPixels;
	unsigned char *shadowPixelsLA; // Luminance + Alpha
	int         nShadowPixels;
	int         shadowW;
	int         shadowH;
	
	
	//---------------------------------------
	GLfloat *materialAmbient;
	GLfloat *materialDiffuse;
	GLfloat *materialSpecular;
	
	
private:
	
};