#include "testApp.h"

void testApp::myInit() {
	//	ofSetFrameRate(60);
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	glEnable (GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//
	light.pointLight(255, 255, 255, ofGetWidth()/2, ofGetHeight(), 1000);
	ofxMaterialSpecular(120, 120, 120); //how much specular light will be reflect by the surface
	ofxMaterialShininess(100); //how concentrated the reflexion will be (between 0 and 128	
	light.specular(255, 255, 255);
	ofxLightsOff();
}

//--------------------------------------------------------------
void testApp::setup(){	 
	myInit();
	
	camera.setup(this, 100);
	
	//	objManager = new ObjManager();	
	//	for (int i=0; i<NUM_SPHERES; i++) {
	//		objManager->createSphere();
	//	}	
	
	objManager = new ObjManager(true);	
	//	for (int i=0; i<NUM_SPHERES; i++) {
	//		objManager->createSoftBody();
	//	}	
	int test = 0;
	//do each sphere update by music or etc.
}

//--------------------------------------------------------------
void testApp::update(){
	//	objManager->update();
	objManager->updateSoftBody();
}

//--------------------------------------------------------------
void testApp::draw(){
	camera.draw();
	ofxLightsOn();
	//	objManager->render();
	objManager->renderSoftBody();
	//ofSaveFrame();
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	btVector3 f;
	switch(key) {
		case 'w':
			f.setX(0);
			f.setY(-10);
			f.setZ(0);		
			objManager->psb->addForce(f);
			break;
		case 'a':
			f.setX(-10);
			f.setY(0);
			f.setZ(0);		
			objManager->psb->addForce(f);
			break;
		case 's':
			f.setX(0);
			f.setY(10);
			f.setZ(0);		
			objManager->psb->addForce(f);
			break;
		case 'd':
			f.setX(10);
			f.setY(0);
			f.setZ(0);		
			objManager->psb->addForce(f);
			break;
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
}
