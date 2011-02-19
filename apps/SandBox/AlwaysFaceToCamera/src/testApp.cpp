#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

		cam.setup(this, 700);
}

//--------------------------------------------------------------
void testApp::update(){


}

//--------------------------------------------------------------
void testApp::draw(){
	
	cam.draw();
	
	ofxLightsOn();
	
	float z = sin( ofGetElapsedTimef() ) * 200.0f;
	
	ofxVec3f center(0, 0, 0);
	//ofxVec3f tar(mouseX- ofGetWidth()*.5, mouseY- ofGetHeight()*.5, z);
	ofxVec3f tar = cam.getEye();
//	ofxVec3f tar = ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, 700);
	
	
	glPushMatrix();
	glTranslatef(ofGetWidth()*.5, ofGetHeight()*.5, 0);
	ofSetColor(255, 0, 0);
	ofxSphere(tar.x, tar.y, tar.z, 50.0f);
	
	
	/***********************/
	ofxVec3f normal = tar - center;
	normal.normalize();
	
	ofxVec3f forward(0, -1, 0);
	ofxVec3f axis   = forward.crossed(normal);
	axis.normalize();
	float angle      = forward.angle(normal);
	
	//not necessary - draw the other axis for perspective //
	ofxVec3f right = axis.crossed(normal);
	right.normalize();
	right *= 250;
	
	// extend the axis so that we can draw it larger //
	ofxVec3f da = axis;
	da *= 250;
	
	glPushMatrix();
	glTranslatef(0, 0, 0);
	
	ofxLightsOff();
	ofSetColor(0, 0, 255);
	ofxLine(center, right);
	
	ofSetColor(0, 255, 0);
	ofxLine(center, da);
	ofxLightsOn();
	glRotatef( angle, axis.x, axis.y, axis.z );
	ofSetColor(150, 150, 150);
	ofxCone(center, ofxVec3f(50, 180, 50) );
	//ofxBox(center, ofxVec3f(50, 50, 50));
	glPopMatrix();
	
	/*********************************************************/
	
	ofxLightsOff();
	
	ofSetColor(255, 255, 255);
	ofxLine(center, tar);
	glPopMatrix();
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

