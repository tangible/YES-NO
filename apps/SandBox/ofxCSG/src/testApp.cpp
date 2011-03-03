#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	// ORBIT VIEW
	prev_x = 0;
	prev_y = 0;
	zoom = 0.0;	
	view_rotx = 0.0f;
    view_roty = 0.0f;
	view_rotz = 0.0f;	
	
	halfWidth = ofGetWidth() * 0.5;
	halfHeight = ofGetHeight() * 0.5;
	
	// Define the model from a set of saved vertices/indices in the data folder
	// Give it a color as well
	solid1.setup("box.txt", 255, 0, 0);
	solid2.setup("box.txt", 255, 255, 0);
	
	// Shift it over a little
	solid2.translate(-0.25, 0.1, 0.1);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	// Animate by moving one of the solids
	float dist = sin(ofGetFrameNum() * 0.01) * 0.001;
	solid2.translate(0, dist, 0);
	// Rotate seems to be very buggy...
	//solid2.rotate(0, 1, 0, dist);
	
	// Must call setup again to define the new solids
	model.setup(&solid1, &solid2);

	// The resulting solid is allocated in memory so must be deleted when done
	//ofxCSGSolid *result = model.getIntersection();
	//ofxCSGSolid *result = model.getUnion();
	ofxCSGSolid *result = model.getDifference();
	
	ofPushMatrix();

	glTranslatef(ofGetWidth()/2, ofGetHeight()/2, 0);
	glScalef(250, 250, 250);

	// Viewing rotation
	glRotatef(view_rotx, 1.0f, 0.0f, 0.0f);
	glRotatef(view_roty, 0.0f, 1.0f, 0.0f);
	glRotatef(view_rotz, 0.0f, 0.0f, 1.0f);
	
	// Draw the boolean solid
	result->draw();

	// Draw the outlines of the originals for reference
	solid1.drawOutline();
	solid2.drawOutline();
	
	ofPopMatrix();
	
	// Must delete this when we are done
	delete result;
	
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
	rotate(x, y);
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

void testApp::resetRotation() {
	view_rotx = 0.0f;
	view_roty = 0.0f;
	view_rotz = 0.0f;
}

void testApp::rotate(int x, int y) {
	
	float xDiff = x - prev_x;
	float yDiff = y - prev_y;
	float thetaZ = 360.0f * (xDiff * 0.001f);
	float thetaX = 360.0f * (yDiff * 0.001f);
	
	prev_x = x;
	prev_y = y;
	
	view_rotx += thetaX;
	view_rotz += thetaZ;
}

