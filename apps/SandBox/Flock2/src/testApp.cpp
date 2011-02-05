#include "testApp.h"
#include "ofxSimpleGuiToo.h"

//--------------------------------------------------------------
void testApp::setup(){
	framenum=0;
	doCapture=false;
	
	ofSetFrameRate(36);
	ofBackground(200, 200, 200);
	
	for(int i=0; i<1; i++) {
		f.addBoid();
	}
	
	bg.loadImage("imgs/vancity.jpg");

}

//--------------------------------------------------------------
void testApp::update(){
	f.update(mouseX, mouseY);
	
}



//--------------------------------------------------------------
void testApp::draw(){
	
	bg.draw(ofGetWidth()/2-bg.getWidth()/2, 
			ofGetHeight()/2-bg.getHeight()/2+200);	
	
	f.draw();
	capture();
	
	gui.draw();	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key=='c') doCapture = !doCapture;
	if (key == ' ') gui.toggleDraw();
	if (key == 's') {

	}
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