#include "App.h"

const int fps = 40;

//--------------------------------------------------------------
void App::setup(){

	ofSetFrameRate(fps);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetDataPathRoot("../Resources/");
	ofBackground(0,0,0);
		
	adminPanel.setup();
	sText.setup();	
	convexHull.setup(fps, &adminPanel, &cam);
	qImage.setup();
	httpClient.setup();
	
	ofAddListener(adminPanel.onFileDialogueBG, this, &App::onFileChangeBG);	
	ofAddListener(adminPanel.onClearBG, this, &App::onClearBG);	
	ofAddListener(adminPanel.onFileDialogueQImg, this, &App::onFileChangeQImg);	
	ofAddListener(adminPanel.onClearQImg, this, &App::onClearQImg);		
	ofAddListener(httpClient.onSMSRecieved, this, &App::onSMSMsgRecieved);	
	
	qImage.changeImgQImg("qimg3.png");
	
}

//--------------------------------------------------------------
void App::update(){
	
	adminPanel.update();
	convexHull.update();
	
}

//--------------------------------------------------------------
void App::draw(){
	
//	bg.draw(ofGetWidth()/2-bg.getWidth()/2, 
//			ofGetHeight()/2-bg.getHeight()/2);		
//	qImage.draw();
//	sText.draw(upInfo);
	
	ofPushMatrix();
	ofTranslate(0, 0, -300);
	convexHull.draw();	
	ofPopMatrix();
	
	
	adminPanel.draw();	
	
}

//--------------------------------------------------------------
void App::keyPressed(int key){

	if (key == 's') {
		httpClient.emulateSMS();
	}else if (key == '1') {
		for (int i = 0; i < 10; i++) {
			httpClient.emulateSMS();
		}
	}else if (key == '5') {
		for (int i = 0; i < 50; i++) {
			httpClient.emulateSMS();
		}
	}else if (key == '0') {
		for (int i = 0; i < 100; i++) {
			httpClient.emulateSMS();
		}
	}else {	
		adminPanel.keyPressed(key);
	}
}

//--------------------------------------------------------------
void App::onFileChangeBG(FileDef& fd) {
	bg.loadImage(fd.path);
}
void App::onClearBG(int& i) {
	bg.clear();
}
void App::onFileChangeQImg(FileDef& fd) {
	qImage.changeImgQImg(fd.path);
}
void App::onClearQImg(int& i) {
	qImage.clear();
}
void App::onSMSMsgRecieved(UpdateInfo& _upInfo) {
	upInfo = _upInfo;
	sText.onSMSReceivedUpdate(upInfo.sms.YesOrNo, upInfo);
	convexHull.feedSMS(upInfo);
}

//--------------------------------------------------------------
void App::keyReleased(int key){}
//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void App::windowResized(int w, int h){}

