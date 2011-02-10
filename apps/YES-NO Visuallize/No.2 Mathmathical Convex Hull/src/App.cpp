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
	
	adminPanel.setup();
	convexHull.setup(fps, &adminPanel, &cam);
	
}

//--------------------------------------------------------------
void App::update(){
	
	adminPanel.update();
	convexHull.update();
	
}

//--------------------------------------------------------------
void App::draw(){
	
	cam.place();
	convexHull.draw();
	
	ofSetupScreen();
	adminPanel.draw();	
	
}

//--------------------------------------------------------------
void App::keyPressed(int key){

	convexHull.keyPressed(key);
	if (key == ' ') adminPanel.toggle();
	
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

