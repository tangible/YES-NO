#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255, 255, 255);
	ofSetFrameRate(60);
	output ="";
	}



//--------------------------------------------------------------
void testApp::draw(){
	
	// ANIMATION TO SHOW THE APP IS BLOCKED BY DIALOG...
	ofFill();
	ofSetColor(0x00ff00);
	ofPushMatrix();
	ofTranslate(10 + ofGetFrameNum()%100, 20, 0);
	ofEllipse(0, 0, 15, 15);
	ofPopMatrix();
	
	// TEXT OUTPUT
	ofSetColor(0x000000);
	ofDrawBitmapString("Press 'O' for an OpenFileDialog", 200, 20);
	ofDrawBitmapString("Press 'S' for a SaveFileDialog", 200, 35);
	ofSetColor(0x0000ff);
	ofDrawBitmapString(output, 200, 60);
}


//--------------------------------------------------------------
void testApp::openFile(){
	// first, create a string that will hold the URL
	string URL;
	
	// openFile(string& URL) returns 1 if a file was picked
	// returns 0 when something went wrong or the user pressed 'cancel'
	int response = ofxFileDialogOSX::openFile(URL);
	if(response){
		// now you can use the URL 
		output = "URL to open: \n "+URL;
	}else {
		output = "OPEN canceled. ";
	}
}


//--------------------------------------------------------------
void testApp::saveFile(){
	// create a string to hold the folder URL
	string folderURL;
	
	// and one for the filename
	string fileName;
	// saveFile(string& folderURL, string& fileName) returns 1 if a folder + file were specified
	// returns 0 when something went wrong or the user pressed 'cancel'
	int response = ofxFileDialogOSX::saveFile(folderURL, fileName);
	if(response){
		// now you can use the folder URL and the filename.
		output = "SAVE: \n "+fileName+"\n to: \n "+folderURL;
	}else {
		output = "SAVE canceled ";
	}
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
 switch (key) {
	 case 'o':
		 openFile();
		 break;
	 case 's':
		 saveFile();
		 break;
	 default:
		 break;
 }
}



