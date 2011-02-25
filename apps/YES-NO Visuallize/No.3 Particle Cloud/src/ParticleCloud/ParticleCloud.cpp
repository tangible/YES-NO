/*
 *  ParticleCloud.cpp
 *  No.3 Particle Cloud
 *
 *  Created by //Makira on 11/02/18.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "ParticleCloud.h"

void ParticleCloud::setup(int _fps, AdminPanel* ap, ofxCamera* cam) {
	
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, 0, 0), false);	
//	bullet->enableRayCastingMouseInteraction(cam, ofxVec3f(250, 100, 0));
	
	fps = _fps;
	adminPanel = ap;
	
	int numObjs = minParticleNum;

	flockYes.setup(1, ofGetWidth()/2-200, ofGetHeight()-300, 300, 150, 10, numObjs); 
	flockNo.setup(1, ofGetWidth()/2-200, ofGetHeight()-300, 300, 150, 10, numObjs); 	
	
	yes.setup(bullet, &flockYes, ap, Obj::YES, numObjs);
	no.setup(bullet, &flockNo, ap, Obj::NO, numObjs);
	
}

void ParticleCloud::update() {
	
	if (adminPanel->TOGGLEMOTION) {
		
		bullet->stepPhysicsSimulation(fps);

		yes.update();		
		yes.computeMovement();
		yes.computeCloudShape();
		
		no.update();		
		no.computeMovement();
		no.computeCloudShape();
		
	}
	
}

void ParticleCloud::draw() {
	
	ofEnableSmoothing();
	
	ofPushMatrix();
	ofTranslate(200, 150, 0);

	yes.draw();
//	yes.drawFlock();
	
	ofPopMatrix();
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, 150, 0);

	no.draw();	
//	no.drawFlock();
	
	ofPopMatrix();
	
//	bullet->render();	
}

void ParticleCloud::debugKeyPress(int key) {

	if (key == 'y') 
		yes.addSMSObj(40);

	if (key == 'n') 
		no.addSMSObj(40);		

}

void ParticleCloud::feedSMS(UpdateInfo upInfo) {

	
	int yesdiff = upInfo.numDiffYes;
	float ydiff = ofClamp(yesdiff, 0, particleNumDiffMax);
	ydiff = ofMap(ydiff, 0, particleNumDiffMax, minParticleNum, maxParticleNum);
	int nodiff = upInfo.numDiffNo;
	float ndiff = ofClamp(nodiff, 0, particleNumDiffMax);
	ndiff = ofMap(ndiff, 0, particleNumDiffMax, minParticleNum, maxParticleNum);	
	yes.getFlock()->changeTrailPointNum(0, 0, ydiff);
	yes.changeParticleObjNum(bullet, ydiff);	
	no.getFlock()->changeTrailPointNum(0, 0, ndiff);
	no.changeParticleObjNum(bullet, ndiff);	
	
//	float particleYesNum = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, ParticleCloud::minParticleNum, ParticleCloud::maxParticleNum);
//	yes.getFlock()->changeTrailPointNum(0, 0, particleYesNum);
//	float particleNoNum = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, ParticleCloud::minParticleNum, ParticleCloud::maxParticleNum);
//	no.getFlock()->changeTrailPointNum(0, 0, particleNoNum);
//	if (upInfo.numTotalYes > upInfo.numTotalNo) {
//		particleNoNum -= 20;
//	}else if (upInfo.numTotalYes < upInfo.numTotalNo) {
//		particleYesNum -= 20;					
//	}	
//	yes.changeParticleObjNum(bullet, particleYesNum);	
//	no.changeParticleObjNum(bullet, particleNoNum);
	
	float sizeObjYesNum = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, minSizeObjNum, maxSizeObjNum);
	yes.changeSizeObjNum(bullet, sizeObjYesNum);
	float sizeObjNoNum = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, minSizeObjNum, maxSizeObjNum);
	no.changeSizeObjNum(bullet, sizeObjNoNum);

	float pObjYesSize = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, 0.8, 1.4);
	yes.particleObjSize = pObjYesSize;
	float pObjNoSize = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, 0.8, 1.4);
	no.particleObjSize = pObjNoSize;	
	if (upInfo.numTotalYes > upInfo.numTotalNo) {
		yes.particleObjSize += 0.1;		
		no.particleObjSize -= 0.1;		
	}else if (upInfo.numTotalYes < upInfo.numTotalNo) {
		yes.particleObjSize -= 0.1;				
		no.particleObjSize += 0.1;
	}
	
	float sizeObjYesSize = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, 0.5, 1.5);
	yes.addedObjSize = sizeObjYesSize;
	float sizeObjNoSize = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, 0.5, 1.5);
	no.addedObjSize = sizeObjNoSize;
	if (upInfo.numTotalYes > upInfo.numTotalNo) {
		yes.addedObjSize += 0.2;	
		no.addedObjSize -= 0.2;			
	}else if (upInfo.numTotalYes < upInfo.numTotalNo) {
		no.addedObjSize += 0.2;				
		yes.addedObjSize -= 0.2;					
	}	
	
	float impulseFYes = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, minImpulseFactor, maxImpulseFactor);
	yes.impulseFactor = impulseFYes;
	float impulseFNo = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, minImpulseFactor, maxImpulseFactor);
	no.impulseFactor = impulseFNo;
	
	float yesColRadius = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, -0.1, 0.2); 
	yes.colorRadius = yesColRadius;
	float noColRadius = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, -0.1, 0.2); 
	no.colorRadius = noColRadius;
	if (upInfo.numTotalYes > upInfo.numTotalNo) {
		yes.colorRadius =  0.1;					
	}else if (upInfo.numTotalYes < upInfo.numTotalNo) {			
		no.colorRadius =  0.1;
	}else {
		yes.colorRadius =  0.0;				
		no.colorRadius =  0.0;		
	}
	
	
	if (upInfo.numTotalYes > upInfo.numTotalNo) {
		yes.getFlock()->setBoidSpeed(0, 0, maxBoidSpeed);
		no.getFlock()->setBoidSpeed(0, 0, minBoidSpeed);
	}else if (upInfo.numTotalYes < upInfo.numTotalNo) {
		yes.getFlock()->setBoidSpeed(0, 0, minBoidSpeed);		
		no.getFlock()->setBoidSpeed(0, 0, maxBoidSpeed);
	}else {
		yes.getFlock()->setBoidSpeed(0, 0, normaBoidSpeed);		
		no.getFlock()->setBoidSpeed(0, 0, normaBoidSpeed);
	}
	
	if (Obj::YES == upInfo.sms.YesOrNo) {
		float size = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, ParticleCloud::minSizeObjSize, ParticleCloud::maxSizeObjSize);
		yes.addSMSObj(40);
	}else if (Obj::NO == upInfo.sms.YesOrNo) {
		float size = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, ParticleCloud::minSizeObjSize, ParticleCloud::maxSizeObjSize);		
		no.addSMSObj(40);
	}

}