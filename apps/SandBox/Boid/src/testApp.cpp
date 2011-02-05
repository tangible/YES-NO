#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetFrameRate(36);
	ofBackground(200, 200, 200);
	
	framenum=0;
	doCapture=false;
	
	for(int i=0; i<100; i++) {
		Boid b;
		boids.push_back( b );
	}
}

//--------------------------------------------------------------
void testApp::update(){
	

	for(int i=0; i<boids.size(); i++)
	{
		if(i<5) {
			boids[i].seek( ofPoint(mouseX, mouseY) );
		} else {
			boids[i].arrive( ofPoint(mouseX, mouseY) );
		}

		boids[i].update();
	}
}

//--------------------------------------------------------------
void testApp::draw(){

	ofNoFill();
	ofCircle(mouseX, mouseY, 20);
	
	for(int i=0; i<boids.size(); i++) {
		boids[i].draw();
	}
	capture();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key=='c') doCapture = !doCapture;
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

//--------------------------------------------------------------
void testApp::capture() {
	if(doCapture && ofGetFrameNum() % 4 == 0)
	{
		char filename[255];
		sprintf(filename, "frame%05d.png", framenum++);
		ofSaveScreen(filename);
	}
}