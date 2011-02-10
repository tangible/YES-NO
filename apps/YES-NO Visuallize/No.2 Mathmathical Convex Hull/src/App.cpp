#include "App.h"

const int fps = 40;

//--------------------------------------------------------------
void App::setup(){

	ofSetFrameRate(fps);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetDataPathRoot("../Resources/");
	ofBackground(100, 100, 100);
	
	cam.position(ofGetWidth()/2, ofGetHeight()/2+200, 1200);
	
	convexHull.setup(fps, &cam);
	
}

//--------------------------------------------------------------
void App::update(){
	
	convexHull.update();
	
}

//--------------------------------------------------------------
void App::draw(){
	
	ofSetColor(200, 200, 200);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);		
	
	cam.place();
	convexHull.draw();
	
}

//--------------------------------------------------------------
void App::keyPressed(int key){

	convexHull.keyPressed(key);
	
}

//--------------------------------------------------------------
void App::keyReleased(int key){}
//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void App::windowResized(int w, int h){}

