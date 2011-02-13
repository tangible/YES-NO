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
	//bullet->enableRayCastingMouseInteraction(cam);
	bullet->createGround(ofxVec3f(ofGetWidth()/2, ofGetHeight()+280, 0), ofxVec3f(2000,500,2000), 0, ofxVec4f(0.9, 0.9, 0.9, 0.1));		
	
	ofxVec3f yesPoint = ofxVec3f(ofGetWidth()/2+500, ofGetHeight()-300, 100);
	ofxVec3f noPoint = ofxVec3f(ofGetWidth()/2-400, ofGetHeight()-440, -50); 
	
	yesSoft.setup(bullet, yesPoint, 512, ofxVec3f(60, 30, 80));
	//noSoft.setup(bullet, noPoint, 512, ofxVec3f(120, 60, 70));
	
	bNewSMS = false;
	ofAddListener(insms.onSmsReached, this, &ConvexHull::onSmsReached);	
}

void ConvexHull::update() {

	bullet->stepPhysicsSimulation(fps);
	
	yesSoft.update();
	noSoft.update();
}

void ConvexHull::draw(int mouseX, int mouseY) {
	
	bullet->ground->render(bullet->getWorld());
	
	setupGLStuff();
	//ofEnableAlphaBlending();
	
	yesSoft.draw();
	//noSoft.draw();
	
	if (bNewSMS) {
		insms.debugDraw();
	}
	
}

void ConvexHull::keyPressed(int key) {

	if (key == 'i') {
		insms.setup(yesSoft.yesORno->getSoftBody()->m_faces);
		bNewSMS = true;
	}
	
	yesSoft.debugKeyPressed(key);
	//noSoft.debugKeyPressed(key);
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

void ConvexHull::onSmsReached(int& faceID) {

	yesSoft.addSMS(faceID);

}