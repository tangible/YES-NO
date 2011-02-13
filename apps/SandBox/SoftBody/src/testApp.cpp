#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	
	cam.position(ofGetWidth()/2, ofGetHeight()/2, 700);
	
	bullet = new ofxBullet();
	bullet->initPhysics(ofxVec3f(0, 0, 0), false);
	bullet->createGround(ofxVec3f(ofGetWidth()/2, ofGetHeight()-200, 0), ofxVec3f(2000,0,2000), 0, ofxVec4f(0.9, 0.9, 0.9, 1.0));	
	
	ofxVec3f gravity(0, 1, 0);
	ofxVec3f center(ofGetWidth()/2, ofGetHeight()/2+150, 0);
	ofxVec3f radius(60, 30, 30);
	int resolusion = 128;
	softy = bullet->createEllipsoid(gravity, center, radius, resolusion);
	
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	bullet->stepPhysicsSimulation(60);
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);
	cam.place();
	
	bullet->ground->render(bullet->getWorld());
	softy->render();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
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
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

