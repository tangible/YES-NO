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
	yes.setup(bullet, ap, Obj::YES, numObjs);
	no.setup(bullet, ap, Obj::NO, numObjs);
	
//	flock.setup(2, ofGetWidth()+400, ofGetHeight()-200, 500, -800, 10, numObjs); 
	flock.setup(2, ofGetScreenWidth()+400, ofGetScreenHeight()-200, 500, -800, 10, numObjs); 	
	
}

void ParticleCloud::update() {
	
	if (adminPanel->TOGGLEMOTION) {
		
		bullet->stepPhysicsSimulation(fps);
		flock.update(true);
		
		yes.computeMovement(flock);
		yes.computeCloudShape(flock);
		yes.update();
		
		no.computeMovement(flock);
		no.computeCloudShape(flock);
		no.update();
	}
	
}

void ParticleCloud::draw() {
	
	
	ofPushMatrix();
	ofTranslate(-200, 100, 0);
	
	yes.draw();
	no.draw();
	
//	flock.draw();
//	bullet->render();
	
//	ofSetColor(255, 255, 255);
//	for (int i = 0; i < flock.flocks.size(); i++) {
//		for (int j = 0; j < flock.flocks[i]->boids.size(); j++) {
//			for (int k = 0; k < flock.flocks[i]->boids[j]->poss.size(); k++) {
//				ofxPoint(flock.flocks[i]->boids[j]->poss[k].x,
//						 flock.flocks[i]->boids[j]->poss[k].y,
//						 flock.flocks[i]->boids[j]->poss[k].z);
//			}
//			
//		}
//	}
		
	
	ofPopMatrix();
}

void ParticleCloud::debugKeyPress(int key) {

	if (key == 'y') 
		yes.addSMSObj(60);

	if (key == 'n') 
		no.addSMSObj(60);		

}

void ParticleCloud::feedSMS(UpdateInfo upInfo) {
	
	float particleYesNum = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, ParticleCloud::minParticleNum, ParticleCloud::maxParticleNum);
	flock.changeTrailPointNum(Obj::YES, yes.getBoidID(), particleYesNum);
	float particleNoNum = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, ParticleCloud::minParticleNum, ParticleCloud::maxParticleNum);
	flock.changeTrailPointNum(Obj::NO, no.getBoidID(), particleNoNum);
	if (upInfo.numTotalYes > upInfo.numTotalNo) {
		particleNoNum -= 20;
//		particleYesNum += 20;
	}else if (upInfo.numTotalYes < upInfo.numTotalNo) {
		particleYesNum -= 20;			
//		particleNoNum += 20;		
	}	
	yes.changeParticleObjNum(bullet, particleYesNum);	
	no.changeParticleObjNum(bullet, particleNoNum);
	
	float sizeObjYesNum = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, minSizeObjNum, maxSizeObjNum);
	yes.changeSizeObjNum(bullet, sizeObjYesNum);
	float sizeObjNoNum = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, minSizeObjNum, maxSizeObjNum);
	no.changeSizeObjNum(bullet, sizeObjNoNum);

	float pObjYesSize = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, 0.8, 1.4);
	yes.particleObjSize = pObjYesSize;
	float pObjNoSize = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, 0.8, 1.4);
	no.particleObjSize = pObjNoSize;	
	if (upInfo.numTotalYes > upInfo.numTotalNo) {
		yes.particleObjSize += 0.2;		
		no.particleObjSize -= 0.2;		
	}else if (upInfo.numTotalYes < upInfo.numTotalNo) {
		yes.particleObjSize -= 0.2;				
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
		flock.setBoidSpeed(Obj::YES, yes.getBoidID(), maxBoidSpeed);
		flock.setBoidSpeed(Obj::NO, no.getBoidID(), minBoidSpeed);
	}else if (upInfo.numTotalYes < upInfo.numTotalNo) {
		flock.setBoidSpeed(Obj::YES, yes.getBoidID(), minBoidSpeed);		
		flock.setBoidSpeed(Obj::NO, no.getBoidID(), maxBoidSpeed);
	}else {
		flock.setBoidSpeed(Obj::YES, yes.getBoidID(), normaBoidSpeed);		
		flock.setBoidSpeed(Obj::NO, no.getBoidID(), normaBoidSpeed);
	}
	
	if (Obj::YES == upInfo.sms.YesOrNo) {
		yes.addSMSObj(60);
	}else if (Obj::NO == upInfo.sms.YesOrNo) {
		no.addSMSObj(60);
	}

}