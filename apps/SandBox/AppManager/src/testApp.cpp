#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	adminPanel.setup();
	ofAddListener(adminPanel.onFileDialogue, this, &testApp::onFileChange);
	
	httpClient.setup();
}

//--------------------------------------------------------------
void testApp::update(){

	adminPanel.update();
	httpClient.update();
}

//--------------------------------------------------------------
void testApp::draw(){

	adminPanel.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch(key) {
		case ' ': adminPanel.toggle(); break;
	}	
	
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

//--------------------------------------------------------------
void testApp::onFileChange(FileDef& fd) {
	cout << fd.path << endl;
}