/*
 *  ConvexHull.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/09.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "ConvexHull.h"

void ConvexHull::setup(int _fps, AdminPanel* _adminPanel, ofxCamera* _cam) {

	fps = _fps;
	admin = _adminPanel;
	cam = _cam;
	
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, -10, 0));	
	
	yesPoint = ofxVec3f(ofGetWidth()/2-400, ofGetHeight()-370, -200);
	noPoint = ofxVec3f(ofGetWidth()/2+400, ofGetHeight()-370, -200); 
	
	currentYesLevel = 0;
	yesSoft.setup(YES, bullet, ofxVec3f(0,0,0), ofxVec3f(130, 70, 70), currentYesLevel);
	currentNoLevel = 0;
	noSoft.setup(NO, bullet, ofxVec3f(0,0,0), ofxVec3f(120, 60, 80), currentNoLevel);
	
	isYesUpdating = false;
	isNoUpdating = false;
	
	ofAddListener(yesSoft.onFinishAllUpdating, this, &ConvexHull::onFinishAllUpdating);
	ofAddListener(noSoft.onFinishAllUpdating, this, &ConvexHull::onFinishAllUpdating);	
	ofAddListener(yesSoft.notifyScaleYesEvent, this, &ConvexHull::scaleYes);
	ofAddListener(noSoft.notifyScaleYesEvent, this, &ConvexHull::scaleYes);	
	ofAddListener(yesSoft.notifyScaleNoEvent, this, &ConvexHull::scaleNo);
	ofAddListener(noSoft.notifyScaleNoEvent, this, &ConvexHull::scaleNo);		
	
	currYesScale = 1.0;
	currNOScale = 1.0;
	yesScaleTween.setParameters(scaleEasing, ofxTween::easeIn, currYesScale, currYesScale, 0, 0);
	noScaleTween.setParameters(scaleEasing, ofxTween::easeIn, currNOScale, currNOScale, 0, 0);	
}

void ConvexHull::update() {

	bullet->stepPhysicsSimulation(fps);
	
	for (int i = 0; i < insmsYes.size(); i++) {
		insmsYes[i]->update(yesSoft.yesORno->getSoftBody()->m_faces);
	}
	for (int i = 0; i < insmsNo.size(); i++) {
		insmsNo[i]->update(noSoft.yesORno->getSoftBody()->m_faces);
	}
	
	yesSoft.update();
	noSoft.update();

}

void ConvexHull::draw() {
	
	setupGLStuff();
	
	ofPushMatrix();
	ofTranslate(yesPoint.x, yesPoint.y, yesPoint.z);
	yesSoft.updateRotateion();

	float yScale = yesScaleTween.update();
	currYesScale = yScale;	
	//ofScale(yScale, yScale, yScale);	
	yesSoft.draw();
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(noPoint.x, noPoint.y, noPoint.z);	
	noSoft.updateRotateion();	
	
	float nScale = noScaleTween.update();
	currNOScale = nScale;	
	//ofScale(nScale, nScale, nScale);
	noSoft.draw();
	ofPopMatrix();
	
	for (int i = 0; i < insmsYes.size(); i++) {
		IncomingSMS *sms = insmsYes[i];
		sms->debugDraw();
	}
	for (int i = 0; i < insmsNo.size(); i++) {
		IncomingSMS *sms = insmsNo[i];
		sms->debugDraw();
	}
	
}

void ConvexHull::scaleYes(int & z) {
	
	float sd = scaleDiffMax;
	int yesdiff = updateInfo.numDiffYes;
	float diff = ofClamp(yesdiff, -sd, sd);
	diff = ofMap(diff, -sd, sd, minScale, maxScale);
	yesScaleTween.setParameters(scaleEasing, ofxTween::easeIn, currYesScale, diff, scaleDurTime, 0);
	
}

void ConvexHull::scaleNo(int & z) {
	
	float sd = scaleDiffMax;
	int nodiff = updateInfo.numDiffNo;
	float diff = ofClamp(nodiff, -sd, sd);
	diff = ofMap(diff, -sd, sd, minScale, maxScale);
	noScaleTween.setParameters(scaleEasing, ofxTween::easeIn, currNOScale, diff, scaleDurTime, 0);
	
}

float ConvexHull::feedSMS(UpdateInfo upInfo) {
	
	updateInfo = upInfo;
	IncomingSMS *sms = new IncomingSMS();
	if (upInfo.sms.YesOrNo == YES) {
		int faceID = ofRandom(0, yesSoft.yesORno->getSoftBody()->m_faces.size());
		for (int i = 0; i < yesSoft.addedSMSs.size(); i++) {
			int fid = yesSoft.addedSMSs[i]->faceID;
			if (i != fid) {
				break;
			}else {
				faceID = ofRandom(0, yesSoft.yesORno->getSoftBody()->m_faces.size());
			}
		}
		sms->setup(YES, yesSoft.yesORno->getSoftBody()->m_faces, faceID);
		insmsYes.push_back(sms);
		isYesUpdating = true;
	}else if (upInfo.sms.YesOrNo == NO) {
		int faceID = ofRandom(0, noSoft.yesORno->getSoftBody()->m_faces.size());
		for (int i = 0; i < noSoft.addedSMSs.size(); i++) {
			int fid = noSoft.addedSMSs[i]->faceID;
			if (i != fid) {
				break;
			}else {
				faceID = ofRandom(0, noSoft.yesORno->getSoftBody()->m_faces.size());
			}
		}
		sms->setup(NO, noSoft.yesORno->getSoftBody()->m_faces, faceID);
		insmsNo.push_back(sms);		
		isNoUpdating = true;
	}
	
	ofAddListener(sms->onSmsReached, this, &ConvexHull::onSmsReached);	
	ofAddListener(sms->onSmsCompleted, this, &ConvexHull::onSmsCompleted);
	
}

void ConvexHull::onSmsReached(SmsInfo& smsInfo) {

	if (smsInfo.YesOrNo == YES) {
		yesSoft.addSMS(smsInfo.faceID, updateInfo.numTotalYes, updateInfo.ratioTotalYes);
		yesSoft.startFaceingToCam(cam, yesPoint+ofxVec3f(0, 300, -500));
	}else if (smsInfo.YesOrNo == NO) {
		noSoft.addSMS(smsInfo.faceID, updateInfo.numTotalNo, updateInfo.ratioTotalNo);
		noSoft.startFaceingToCam(cam, noPoint+ofxVec3f(0, 300, -500));				
	}

}

void  ConvexHull::onSmsCompleted(SmsInfo& smsInfo) {

	if (YES == smsInfo.YesOrNo) {
		for (int i = 0; i < insmsYes.size(); i++) {
			int faceID = smsInfo.faceID;
			int fid = insmsYes[i]->faceID;
			if (faceID == fid) {
				IncomingSMS* del = insmsYes[i];
				delete del;
				insmsYes.erase(insmsYes.begin()+i);
			}
		}
		yesSoft.updateInfo = updateInfo;
	}else if (NO == smsInfo.YesOrNo) {
		for (int i = 0; i < insmsNo.size(); i++) {
			int faceID = smsInfo.faceID;
			int fid = insmsNo[i]->faceID;
			if (faceID == fid) {
				IncomingSMS* del = insmsNo[i];
				delete del;
				insmsNo.erase(insmsNo.begin()+i);
			}
		}
		noSoft.updateInfo = updateInfo;
	}
		
	
}

void ConvexHull::onFinishAllUpdating(int & yesOrNo) {
	
	if (yesOrNo == YES) isYesUpdating = false;
	if (yesOrNo == NO) isNoUpdating = false;
	cout << "got event" << endl;
}

void ConvexHull::setupGLStuff(){
	
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
	
    glDisable(GL_BLEND);
    glPolygonMode(GL_BACK, GL_FILL );
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glShadeModel(GL_SMOOTH);
	
    //glColor3f(admin->BASECOL[0], admin->BASECOL[1], admin->BASECOL[2]);
    GLfloat on[]  = {1.0};
    GLfloat off[] = {0.0};
    glLightModelfv( GL_LIGHT_MODEL_TWO_SIDE, on);
	
    GLfloat shininess[] = {admin->MATERIALSHINENESS};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, admin->MATERIALAMBIENT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, admin->MATERIALDIFFUSE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, admin->MATERIALSPECULAR);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	
	GLfloat lightPosition[] = {admin->LIGHTX, admin->LIGHTY, admin->LIGHTZ, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, admin->LIGHTDIFFUSE);
    glLightfv(GL_LIGHT0, GL_SPECULAR, admin->LIGHTSPECULAR);
    glLightfv(GL_LIGHT0, GL_AMBIENT, admin->LIGHTAMBIENT);	
	
}