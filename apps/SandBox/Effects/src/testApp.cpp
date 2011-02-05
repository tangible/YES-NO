#include "testApp.h"

int numImgs = 1;
//--------------------------------------------------------------
void testApp::setup(){

	ofEnableAlphaBlending();
	ofBackground(255, 255, 255);
	
	for (int i = 0; i < numImgs; i++) {
		
		Effect* ef = new Effect();
		
		ofImage img;
		img.loadImage("img/"+ofToString(i)+".png");
		ef->img = img;
		
		float rdmScale = ofRandomuf();
		ofxVec2f scale = ofxVec2f(rdmScale, rdmScale);
		ef->scale = scale;
		ef->scaleTween.setParameters(ef->easingbounce, ofxTween::easeInOut, 0.0, rdmScale, 0, 0);
		
		ofxVec3f pos = ofxVec3f(ofGetWidth()/2, ofGetHeight()/2, 0);
		ef->pos = pos;
		
		ofxVec4f col = ofxVec4f(ofRandomuf(), ofRandomuf(), ofRandomuf(), ofRandomuf());
		ef->col = col;
		
		effects.push_back(ef);
	}

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(0, 0, 0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);
	
	ofSetColor(200, 255, 255);
	for (int i = 0; i < numImgs; i++) {
		Effect* ef = effects[i];
		ofImage img = ef->img;
		
		ofPushMatrix();
		glColor4f(ef->col.x, ef->col.y, ef->col.z, ef->col.w);
		ofTranslate(ef->pos.x-img.getWidth()/2*ef->scale.x, 
					ef->pos.y-img.getHeight()/2*ef->scale.x, 
					0);
		ofScale(ef->scale.x, ef->scale.x, 0);

		img.draw(0, 0);;
		ofPopMatrix();
	}
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

