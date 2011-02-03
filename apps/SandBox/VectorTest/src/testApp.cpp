#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	for (int i = 0; i < 2; i++) {
		MyClass* mc = new MyClass(i);
		mc->myClassID = ofToString(i)+"class";
		for (int j = 0; j < 10; j++) {
			MySubClass* msc = new MySubClass();
			msc->mySubClassID = ofToString(j)+"subclass";
			mc->mscptr.push_back(msc);
			mc->mscptrlist.push_back(msc);
		}
		mcptr.push_back(mc);
		mcptrlist.push_back(mc);
	}
	
	int test = 0;
	
}

//--------------------------------------------------------------
void testApp::update(){
	int test = 0;
}

//--------------------------------------------------------------
void testApp::draw(){

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

