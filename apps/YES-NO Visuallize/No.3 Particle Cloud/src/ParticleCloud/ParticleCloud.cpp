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

	flockYes.setup(1, ofGetScreenWidth()/2-200, ofGetScreenHeight()-300, 300, 150, 10, numObjs); 
	flockNo.setup(1, ofGetScreenWidth()/2-200, ofGetScreenHeight()-300, 300, 150, 10, numObjs); 	
	
	yes.setup(bullet, &flockYes, ap, Obj::YES, numObjs);
	no.setup(bullet, &flockNo, ap, Obj::NO, numObjs);
	
	soundYes.loadSound("key_omin_a.aif");
	soundNo.loadSound("A_kick.aif");	
	
	yesScaleTween.setParameters(scaleEasing, ofxTween::easeIn, 1.0, 1.0, 0, 0);
	noScaleTween.setParameters(scaleEasing, ofxTween::easeIn, 1.0, 1.0, 0, 0);	
	
}

void ParticleCloud::update() {
	
	if (adminPanel->TOGGLEMOTION) {
		
		bullet->stepPhysicsSimulation(fps);

		yes.update();		
		yes.computeCloudShape();
		
		no.update();		
		no.computeCloudShape();
	
	}
	
}

void ParticleCloud::draw() {
	
	ofEnableSmoothing();
	
	setupGLStuff();
	
	ofPushMatrix();
	yes.draw();
	ofPopMatrix();
	
	ofPushMatrix();
	no.draw();	
	ofPopMatrix();
	
}

void ParticleCloud::debugKeyPress(int key) {

	if (key == 'y') 
		yes.addSMSObj(30);

	if (key == 'n') 
		no.addSMSObj(30);		

}

void ParticleCloud::feedSMS(UpdateInfo upInfo) {

	updateInfo = upInfo;
	int yesdiff = upInfo.numDiffYes;
	float ydiff = ofClamp(yesdiff, 0, particleNumDiffMax);
	ydiff = ofMap(ydiff, 0, particleNumDiffMax, minParticleNum, maxParticleNum);
	int nodiff = upInfo.numDiffNo;
	float ndiff = ofClamp(nodiff, 0, particleNumDiffMax);
	ndiff = ofMap(ndiff, 0, particleNumDiffMax, minParticleNum, maxParticleNum);	
	yes.getFlock()->changeTrailPointNum(0, 0, ydiff);
	no.getFlock()->changeTrailPointNum(0, 0, ndiff);
	
	int minthresh = -400/5;
	int maxthresh = 400/5;
	int diffYes = ofClamp(upInfo.numDiffYes, minthresh, maxthresh);
	int diffNo = ofClamp(upInfo.numDiffNo, minthresh, maxthresh);
	float sizeObjYesNum = ofMap(diffYes, minthresh, maxthresh, 400/6, 400);
	yes.changeSizeObjNum(bullet, sizeObjYesNum);
	float sizeObjNoNum = ofMap(diffNo, minthresh, maxthresh, 400/6, 400);
	no.changeSizeObjNum(bullet, sizeObjNoNum);	
	
	float impulseFYes = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, minImpulseFactor, maxImpulseFactor);
	yes.impulseFactor = impulseFYes;
	float impulseFNo = ofMap(upInfo.ratioTotalNo, 0.0, 1.0, minImpulseFactor, maxImpulseFactor);
	no.impulseFactor = impulseFNo;
	
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
	
	int z = 0;
	if (Obj::YES == upInfo.sms.YesOrNo) {
		float size = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, ParticleCloud::minSizeObjSize, ParticleCloud::maxSizeObjSize);
		yes.addSMSObj(ofRandom(30, 40));
		cout << "yes size = "+ofToString((int)yes.getAddeObjNum()) << endl;
		soundYes.play();
	}else if (Obj::NO == upInfo.sms.YesOrNo) {
		float size = ofMap(upInfo.ratioTotalYes, 0.0, 1.0, ParticleCloud::minSizeObjSize, ParticleCloud::maxSizeObjSize);		
		no.addSMSObj(ofRandom(30, 40));
		cout << "no size = "+ofToString((int)no.getAddeObjNum()) << endl;		
		soundNo.play();
	}

}


void ParticleCloud::setupGLStuff(){
	
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
	
    glShadeModel(GL_SMOOTH);
	
    GLfloat on[]  = {1.0};
    GLfloat off[] = {0.0};
    glLightModelfv( GL_LIGHT_MODEL_TWO_SIDE, on);
	
    GLfloat shininess[] = {adminPanel->MATERIALSHINENESS};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, adminPanel->MATERIALAMBIENT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, adminPanel->MATERIALDIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, adminPanel->MATERIALSPECULAR);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	GLfloat lightPosition[] = {adminPanel->LIGHTX, adminPanel->LIGHTY, adminPanel->LIGHTZ, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, adminPanel->LIGHTDIFFUSE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, adminPanel->LIGHTSPECULAR);
    glLightfv(GL_LIGHT0, GL_AMBIENT, adminPanel->LIGHTAMBIENT);	
	
}