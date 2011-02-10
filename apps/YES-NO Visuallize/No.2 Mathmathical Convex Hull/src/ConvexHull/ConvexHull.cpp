/*
 *  ConvexHull.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/09.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "ConvexHull.h"

void ConvexHull::setup(int _fps, ofxCamera* _cam) {

	fps = _fps;
	cam = _cam;
	
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, 0, 0));	
	bullet->enableRayCastingMouseInteraction(cam);
	
	bullet->createGround(ofxVec3f(ofGetWidth()/2, ofGetHeight()-200, 0), ofxVec3f(1500,0,1600), 0, ofxVec4f(0.9, 0.9, 0.9, 0.1));
	
	ofxVec3f yesPoint = ofxVec3f(ofGetWidth()/2+300, ofGetHeight()-200, 100);
	ofxVec3f noPoint = ofxVec3f(ofGetWidth()/2-400, ofGetHeight()-200, -50); 
	
	yes.setup(bullet, yesPoint, 10);
	no.setup(bullet, noPoint, 15);
	
//	lScat.setup(0.75, 0.75, 0.75, 0.8, 
//				ofxVec2f(1.0-ofMap(yesPoint.x, 0.0, ofGetScreenWidth(), 0.0, 1.0), 1.0-ofMap(yesPoint.y, 0.0, ofGetScreenHeight(), 0.0, 1.0)));
	
	// settings for obj vis
    materialAmbient  = new float[4];
    materialDiffuse  = new float[4];
    materialSpecular = new float[4];
    for (int i=0; i<4; i++){
        materialAmbient[i]  = 1.0f;
        materialDiffuse[i]  = 1.0f;
        materialSpecular[i] = 1.0f;
    }	

}

void ConvexHull::update() {

	bullet->stepPhysicsSimulation(fps);

	yes.update();
	no.update();
	
//	lScat.beginFeedingTargetObjDuringYourUpdateCycle();
//	
//	lScat.endFeedingTargetObjDuringYourUpdateCycle();		
	
}

void ConvexHull::draw() {
	
	bullet->ground->render(bullet->getWorld());
	
	setupGLStuff();
	
	//ofEnableAlphaBlending();
//	lScat.drawScatteringObjInYourDrawCycleButBeforeCameraUpdate(7);	
		
	no.draw(ofxVec4f(mc(198), mc(255), mc(133), 1.0));
	yes.draw(ofxVec4f(mc(250), 0, mc(227), 1.0));		
	
//	no.draw(ofxVec4f(198, 255, 133, 255));
//	yes.draw(ofxVec4f(250, 0, 227, 255));152, 0, 144
	
	//yes.draw();73, 144, 137
	//yes.debugDraw();

}

void ConvexHull::keyPressed(int key) {

	if (key == 's') {
		yes.addSMS(1);
	}else if (key == 'a') {
		no.addSMS(1);
	}

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
	
    glColor3f(0.5, 0.5, 0.5);
    GLfloat on[]  = {1.0};
    GLfloat off[] = {0.0};
    glLightModelfv( GL_LIGHT_MODEL_TWO_SIDE,on);
	
	
    GLfloat shininess[] = {0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
	
	
    GLfloat lightPosition[] = { 100.0f, 0.0f, 100.0, 0.0f };
    GLfloat lightDiffuse[]  = { 1.00, 0.99, 0.98, 1.0};
    GLfloat lightSpecular[] = { 0.10, 0.10, 0.10, 1.0};
    GLfloat lightAmbient[]  = { 0.45, 0.43, 0.44, 1.0};
    glLightfv(GL_LIGHT0,GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0,GL_AMBIENT,  lightAmbient);
	
}