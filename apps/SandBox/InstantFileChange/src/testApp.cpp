#include "testApp.h"
#include "ofxSimpleGuiToo.h"

bool changeImgBtn = false;

//--------------------------------------------------------------
void testApp::setup(){
	
	gui.addTitle("A group");
	gui.addButton("change img", changeImgBtn);
	gui.loadFromXML();
	gui.show();
	
}

//--------------------------------------------------------------
void testApp::update(){
	vid.idleMovie();
	
	if(changeImgBtn) {
		changeImgBtn = false;
		openFile();
	}	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofDrawBitmapString("URL to open: \n "+filePath, 10, 20);
	img.draw(100, 100);
	vid.draw(100, 100);
	gui.draw();
	
}

//--------------------------------------------------------------
void testApp::openFile(){
	string path;
	int response = ofxFileDialogOSX::openFile(path);
	if(response){ 
		filePath = path;
		
		bool bLoodImg = img.loadImage(path);
		if (!bLoodImg) {
			filePath = "load img fail!! Trying load as Vid... ";
			bool bLoodVid = vid.loadMovie(path);
			if (!bLoodVid) {
				filePath += "can't load this filetype!";
			}else {
				filePath = path;
				img.clear();
				vid.play();
			}
		}else {
//			vid.stop();
//			vid.closeMovie();
//			vid.close();
		}

		
	}else {
		filePath = "";
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch(key) {
		case ' ': gui.toggleDraw(); break;
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

