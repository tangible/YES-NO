/*
 *  ConvexHull.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/09.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "ConvexHull.h"

void ConvexHull::setup(int _fps, AdminPanel* _adminPanel, StateText* _sText, ofxCamera* _cam) {

	fps = _fps;
	admin = _adminPanel;
	cam = _cam;
	sText = _sText;
	
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, -10, 0));	
	//bullet->enableRayCastingMouseInteraction(cam);
	//bullet->createGround(ofxVec3f(ofGetWidth()/2, ofGetHeight()+280, 0), ofxVec3f(2000,500,2000), 0, ofxVec4f(0.9, 0.9, 0.9, 0.1));		
	
	yesPoint = ofxVec3f(ofGetWidth()/2+500, ofGetHeight()-300, 100);
	noPoint = ofxVec3f(ofGetWidth()/2-400, ofGetHeight()-440, -50); 
	
	currentYesLevel = 1;
	yesSoft.setup(YES, bullet, yesPoint, ofxVec3f(20, 60, 80), currentYesLevel);
	currentNoLevel = 10;
	noSoft.setup(NO, bullet, noPoint, ofxVec3f(120, 60, 70), currentNoLevel);
	
	ofAddListener(yesSoft.onTheEnd, this, &ConvexHull::onTheEnd);
	
	bNewSMS = false;
	
		debugRun = false;
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
	
	if (debugRun) {
		if (ofGetFrameNum()%5 == 0) {
			addSMS(YES);
			addSMS(NO);
		}
	}else {
//		if (ofGetFrameNum()%100 == 0) {
//			addSMS(YES);
//			addSMS(NO);
//		}	
	}
}

void ConvexHull::draw(int mouseX, int mouseY) {
	
	//bullet->ground->render(bullet->getWorld());
	
	setupGLStuff();
	//ofEnableAlphaBlending();
	
	yesSoft.draw();
	noSoft.draw();
	
	for (int i = 0; i < insmsYes.size(); i++) {
		IncomingSMS *sms = insmsYes[i];
		sms->debugDraw();
	}
	for (int i = 0; i < insmsNo.size(); i++) {
		IncomingSMS *sms = insmsNo[i];
		sms->debugDraw();
	}
	
	UpdateInfo upInfo;
	upInfo.numTotalYes = insmsYes.size();
	upInfo.numTotalNo = insmsNo.size();
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	sText->draw(upInfo, yesSoft.yesORno->getBodyCentroid(), noSoft.yesORno->getBodyCentroid());
	glDisable(GL_LIGHTING);	
}

void ConvexHull::keyPressed(int key) {
	
	if (key == 'd') 
		debugRun = !debugRun;
	
	if (key == 'i') {
		addSMS(YES);
		addSMS(NO);
	}
	
	if (key == 't') {
		noSoft.yesORno->getSoftBody()->translate(btVector3(100, 100, 100));
	}
	
//	yesSoft.debugKeyPressed(key);
//	noSoft.debugKeyPressed(key);
}

float ConvexHull::addSMS(int YesOrNo) {
	
	IncomingSMS *sms = new IncomingSMS();
	if (YesOrNo == YES) {
		int faceID = ofRandom(0, yesSoft.yesORno->getSoftBody()->m_faces.size());
		for (int i = 0; i < yesSoft.addedSMSs.size(); i++) {
			int fid = yesSoft.addedSMSs[i]->faceID;
			if (i != fid) {
				break;
			}else {
				faceID = ofRandom(0, yesSoft.yesORno->getSoftBody()->m_faces.size());
			}
		}
		if (yesSoft.yesORno->getSoftBody()->m_faces.size() == yesSoft.addedSMSs.size()) {
			yesSoft.finish();
			cout << "complete!" << endl;
		}
		sms->setup(YES, yesSoft.yesORno->getSoftBody()->m_faces, faceID);
		insmsYes.push_back(sms);
	}else if (YesOrNo == NO) {
		int faceID = ofRandom(0, noSoft.yesORno->getSoftBody()->m_faces.size());
		for (int i = 0; i < noSoft.addedSMSs.size(); i++) {
			int fid = noSoft.addedSMSs[i]->faceID;
			if (i != fid) {
				break;
			}else {
				faceID = ofRandom(0, noSoft.yesORno->getSoftBody()->m_faces.size());
			}
		}
		if (noSoft.yesORno->getSoftBody()->m_faces.size() == noSoft.addedSMSs.size()) {
			noSoft.finish();
			cout << "complete!" << endl;
		}
		sms->setup(NO, noSoft.yesORno->getSoftBody()->m_faces, faceID);
		insmsNo.push_back(sms);		
	}
	
	ofAddListener(sms->onSmsReached, this, &ConvexHull::onSmsReached);	
	ofAddListener(sms->onSmsCompleted, this, &ConvexHull::onSmsCompleted);
	bNewSMS = true;	
	
}

float ConvexHull::mc(float num) {
	return ofMap(num, 0.0, 255.0, 0.0, 1.0);
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

void ConvexHull::onSmsReached(SmsInfo& smsInfo) {

	if (smsInfo.YesOrNo == YES) {
		yesSoft.addSMS(smsInfo.faceID);
	}else if (smsInfo.YesOrNo == NO) {
		noSoft.addSMS(smsInfo.faceID);
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
		
		vector<ofxVec3f> pos;
		pos.push_back(smsInfo.a);
		pos.push_back(smsInfo.b);
		pos.push_back(smsInfo.c);
		yesSoft.addSMSCompleted(smsInfo.faceID, pos);
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
		
		vector<ofxVec3f> pos;
		pos.push_back(smsInfo.a);
		pos.push_back(smsInfo.b);
		pos.push_back(smsInfo.c);
		noSoft.addSMSCompleted(smsInfo.faceID, pos);
	}
		
	
}

void ConvexHull::onTheEnd(int& YesOrNo) {
	
	if (YES == YesOrNo) {
		currentYesLevel++;
		yesSoft.clear();
		yesSoft.setup(YES, bullet, yesPoint, ofxVec3f(60, 30, 80), currentYesLevel);	
		for (int i = 0; i < insmsYes.size(); i++) {
			IncomingSMS* s = insmsYes[i];
			delete s;
		}
		insmsYes.clear();
	}else if (NO == YesOrNo) {
		currentNoLevel++;
		noSoft.clear();
		noSoft.setup(NO, bullet, noPoint, ofxVec3f(120, 60, 70), currentNoLevel);	
		for (int i = 0; i < insmsNo.size(); i++) {
			IncomingSMS* s = insmsNo[i];
			delete s;
		}
		insmsNo.clear();		
	}
}