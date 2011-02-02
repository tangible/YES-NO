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
#include "MetaBallChunk.h"
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
	
	vector<MetaBallChunk*>  mBallChunks;
	int						nPoints;
	int						nMetaBalls;
	ofPoint					boundsAvg;
	float					boundsScaling;
	
	int						blurValue;
	int						counter;
	int						counter2;
	float					maxDeceleration;
	
	float*					screenDepth;   
	int						nScreenPixels;  
	int						screenW;        
	int						screenH;        
	ofTexture				shadowTex;      
	ofxCvGrayscaleImage		shadowCvImage;
	int						shadowDivX;     
	int						shadowDivY;
	unsigned char*			shadowPixels;
	unsigned char*			shadowPixelsLA; 
	int						nShadowPixels;
	int						shadowW;
	int						shadowH;
	
};