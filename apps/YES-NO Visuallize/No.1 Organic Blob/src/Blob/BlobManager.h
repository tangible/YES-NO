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
#include "Flock.h"
#include "QuestionImage.h"
#include "StateText.h"
#include "Ray.h"

class BlobManager {

public:
	void setup(int _fps, AdminPanel* _admin, QuestionImage* _qImage, StateText* _sText);
	void update();
	void draw();
	
	void moveBG();
	void setupGLStuff();
	
	// file change events
	void changeImgBG(string path);
	void changeImgBlobTex(string path);	
	void recieveSMS(UpdateInfo upInfo);	
	
	// sms events
	UpdateInfo upInfo;
	vector<Ray*>  rays;
	vector<Flock*> flocks; // this is sms
	MetaBallChunk* smsYes;
	vector<MyRigidBody*> smsYesBody;
	MetaBallChunk* smsNo;
	MyRigidBody* smsNoBody;
	float thisSizeY;
	float thisSizeN;
	void onSMSRecievedChangeSphereSize(int _chunkID, float totalRatioYes, float totalRatioNo);
	void onSMSRecievedImpulseForSphere(int _chunkID);
	void onBallGetSMSrep(int& chunkID);
	void onBallGetSMSrepComplete(int& chunkID);
	
	AdminPanel*				admin;
	QuestionImage*			qImage;
	StateText*				sText;
	
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
	vector<vector<float> >	sphrersForChunkBaseSize;
	int						randomImpulsSMSRecievedYes;
	int						randomImpulsSMSRecievedNo;
	vector<MetaBallChunk*>  mBallChunks;
	ofxVec3f				centroidYes;
	ofxVec3f				centroidNo;	
	int						nMetaBalls;
	
	ofPoint					boundsAvg;
	float					boundsScaling;
	
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
	
	
	ofImage timg;
	
};