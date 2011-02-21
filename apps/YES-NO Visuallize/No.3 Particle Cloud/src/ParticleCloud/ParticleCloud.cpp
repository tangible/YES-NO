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
	
	int numObjs = 300;
	yes.setup(bullet, ap, Obj::YES, numObjs);
	no.setup(bullet, ap, Obj::NO, numObjs);
	
	// trails for yes and no objs
	flock.setup(2, ofGetWidth()-500, ofGetHeight()-200, 400, -200, 10, numObjs); 
	
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
	ofTranslate(250, 100, 0);
	
	yes.draw();
	no.draw();
	
//	flock.draw();
//	bullet->render();
	
	ofPopMatrix();
}

void ParticleCloud::debugKeyPress(int key) {

	if (key == 'y') 
		yes.addObj();

	if (key == 'n') 
		no.addObj();		

}