#include "App.h"

const int fps = 40;

//--------------------------------------------------------------
void App::setup(){

	ofSetFrameRate(fps);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetDataPathRoot("../Resources/");
	ofBackground(0,0,0);
		
	//cam.position(ofGetWidth()/2, ofGetWidth()/2, 700);
	cam.setup(this, 1000);
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
	prevOrbit = 2;
}

//--------------------------------------------------------------
void App::update(){
	
	adminPanel.update();
	convexHull.update();
	
	cam.orbitAround(cam.getEye(), ofxVec3f(0,1,0), camOrbitTween.update());
	
	if (!convexHull.isYesUpdating && !convexHull.isNoUpdating && smsQue.size() > 0) {
		
		UpdateInfo upInfo = smsQue[0];
		smsQue.erase(smsQue.begin());
		
		sText.onSMSReceivedUpdate(upInfo.sms.YesOrNo, upInfo);
		convexHull.feedSMS(upInfo);
		
		if (prevOrbit != upInfo.sms.YesOrNo) {
			float oamt = 0.0;
			if (upInfo.sms.YesOrNo == 0) {
				oamt = -camOrbitAmt;
			}else {
				oamt = camOrbitAmt;
			}
			if (prevOrbit != 2) oamt *= 2;
			camOrbitTween.setParameters(camOrbitEasing, ofxTween::easeIn, oamt, 0.0, 600, 0);
		}
		prevOrbit = upInfo.sms.YesOrNo;	
	}
}

//--------------------------------------------------------------
void App::draw(){
	
	
//	bg.draw(ofGetWidth()/2-bg.getWidth()/2, 
//			ofGetHeight()/2-bg.getHeight()/2);		
//	qImage.draw();
//	sText.draw(upInfo);
	
//	cam.place();
	cam.draw();
	ofPushMatrix();
//	ofTranslate(0, 0, -300);
	convexHull.draw();	
	ofPopMatrix();
	
	
	adminPanel.draw();	
	ofSetColor(255, 255, 255);	
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
	smsQue.push_back(_upInfo);
//	sText.onSMSReceivedUpdate(upInfo.sms.YesOrNo, upInfo);
//	convexHull.feedSMS(upInfo);
//	
//	if (prevOrbit != upInfo.sms.YesOrNo) {
//		float oamt = 0.0;
//		if (upInfo.sms.YesOrNo == 0) {
//			oamt = -camOrbitAmt;
//		}else {
//			oamt = camOrbitAmt;
//		}
//		if (prevOrbit != 2) oamt *= 2;
//		camOrbitTween.setParameters(camOrbitEasing, ofxTween::easeIn, oamt, 0.0, 400, 0);
//	}
//	prevOrbit = upInfo.sms.YesOrNo;
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

