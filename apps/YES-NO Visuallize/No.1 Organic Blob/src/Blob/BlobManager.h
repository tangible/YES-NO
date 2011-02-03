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
#include "ofxTween.h"
#include "AdminPanel.h"
#include "HTTPClient.h"
#include "MetaBallChunk.h"
#include "Metaballs.h"
#include "Marchingcubes.h"

class BlobManager {

public:
	void setup(int _fps, AdminPanel* _admin);
	void update();
	void draw();
	
	void changeImgBG(string path);
	void changeImgBlobTex(string path);	
	void recieveSMS(UpdateInfo upInfo);
	void moveBG();
	
	void setupGLStuff();
	
	AdminPanel*				admin;
	
	int						fps;
	ofxShader				shader;
	int						texSlot;	
	ofVideoPlayer			bgPlayer;
	bool					isVidBG;
	ofVideoPlayer			blobTexPlayer;
	bool					isVidBlobTex;	
	ofImage					bg;
	ofxVec3f				bgCenter;
	ofxTween				bgXTween;
	ofxTween				bgYTween;
	ofImage					blobTex;
	
	ofxBullet*				bullet;
	vector<vector<MyRigidBody*> > sphrersForChunk;
	vector<MetaBallChunk*>  mBallChunks;
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
	
	
	// tweens
	ofxEasingBack		easingback;
	ofxEasingBounce 	easingbounce;
	ofxEasingCirc		easingcirc;
	ofxEasingCubic		easingcubic;
	ofxEasingElastic	easingelastic;
	ofxEasingExpo		easingexpo;
	ofxEasingLinear 	easinglinear;
	ofxEasingQuad		easingquad;
	ofxEasingQuart		easingquart;
	ofxEasingQuint		easingquint;
	ofxEasingSine		easingsine;
	
};