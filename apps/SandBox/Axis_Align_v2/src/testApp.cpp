#include "testApp.h"

static void drawCircle( const ofxVec3f& $v ) {
	glPushMatrix();
	glTranslatef($v.x, $v.y, $v.z);
	ofCircle(0, 0, 25);
	glPopMatrix();
}

static void drawLine( const ofxVec3f& $v1, const ofxVec3f& $v2) {
	glBegin(GL_LINES);
	glVertex3d($v1.x, $v1.y, $v1.z);
	glVertex3d($v2.x, $v2.y, $v2.z);
	glEnd();
}

//--------------------------------------------------------------
void testApp::setup(){
	glEnable(GL_DEPTH_TEST);
	ofBackground(0, 0, 0);
	
	ofxMaterialSpecular(120, 120, 120);
	ofxMaterialShininess( 50 );
	
	light.specular(255, 255, 255);
}

//--------------------------------------------------------------
void testApp::update(){
	light.pointLight(255, 255, 255, ofGetWidth()*.5, ofGetWidth()*.5, 300);
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofxLightsOn();
	
	float z = sin( ofGetElapsedTimef() ) * 200.0f;
	
	ofxVec3f center(0, 0, 0);
	ofxVec3f tar(mouseX- ofGetWidth()*.5, mouseY- ofGetHeight()*.5, z);
	
	
	glPushMatrix();
	glTranslatef(ofGetWidth()*.5, ofGetHeight()*.5, 0);
	ofSetColor(255, 0, 0);
	ofxSphere(tar.x, tar.y, tar.z, 50.0f);
	
	
	/***********************/
	ofxVec3f normal = tar - center;
	normal.normalize();
	
	ofxVec3f forward(0, 1, 0);
	ofxVec3f axis	= forward.crossed(normal);
	axis.normalize();
	float angle		= forward.angle(normal);
	
	// set Quaternion
	ofxQuaternion quat(axis.x, axis.y, axis.z, ofDegToRad(angle));
	
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
	drawLine(center, right);
	
	ofSetColor(0, 255, 0);
	drawLine(center, da);
	ofxLightsOn();
	
	// get rot from Quaternion
	ofxVec3f qaxis; float qangle;
	quat.getRotate(qangle, qaxis);	
	glRotatef(ofRadToDeg(qangle), -qaxis.x, qaxis.y, -qaxis.z);	

	ofSetColor(150, 150, 150);
	ofxCone(center, ofxVec3f(50, 180, 50) );
	//ofxBox(center, ofxVec3f(50, 50, 50));
	glPopMatrix();
	
	/*********************************************************/
	
	
	ofxLightsOff();
	
	ofSetColor(255, 255, 255);
	drawLine(center, tar);
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

