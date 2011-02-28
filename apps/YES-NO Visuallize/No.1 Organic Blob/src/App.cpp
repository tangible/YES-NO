#include "App.h"

//--------------------------------------------------------------
void App::setup(){
	
	ofSetFrameRate(FPS);
	ofBackground(0,0,0);
	ofSetDataPathRoot("../Resources/");

	adminPanel.setup();
	qImage.setup();
	sText.setup();
	blobMgr.setup(FPS, &adminPanel, &camera, &qImage, &sText);
	httpClient.setup();
	
	ofAddListener(adminPanel.onFileDialogueBG, this, &App::onFileChangeBG);
	ofAddListener(adminPanel.onFileDialogueQImg, this, &App::onFileChangeQImg);	
	ofAddListener(adminPanel.onFileDialogueBlobTex, this, &App::onFileChangeBlobTex);	
	ofAddListener(httpClient.onSMSRecieved, this, &App::onSMSMsgRecieved);
	ofAddListener(adminPanel.onClearQImg, this, &App::onClearQImg);		
	
	camera.position(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 1060);
	cameraXTween.setParameters(easingback, ofxTween::easeOut, ofGetScreenWidth()/2, ofGetScreenWidth()/2, 0, 0);
	eyeXTween.setParameters(easingback, ofxTween::easeOut, ofGetScreenWidth()/2, ofGetScreenWidth()/2, 0, 0);
	cameraYTween.setParameters(easingback, ofxTween::easeOut, ofGetScreenHeight()/2, ofGetScreenHeight()/2, 0, 0);
	eyeYTween.setParameters(easingback, ofxTween::easeOut, ofGetScreenHeight()/2, ofGetScreenHeight()/2, 0, 0);
	cameraZTween.setParameters(easingback, ofxTween::easeOut, 700, 700, 0, 0);
	
	qImage.changeImgQImg("qimg3.png");
	
}

//--------------------------------------------------------------
void App::update(){
	
	blobMgr.update();
	adminPanel.update();
	httpClient.update();
	
//	jitterAndZoomScene();
//	camera.position(cameraXTween.update(), cameraYTween.update(), cameraZTween.update());
//	camera.eye(eyeXTween.update(), eyeYTween.update(), 0);
}

//--------------------------------------------------------------
void App::draw(){
	
	ofPushMatrix();
	camera.place();
	blobMgr.draw();		
	ofPopMatrix();
	
	ofSetupScreen();	
	adminPanel.draw();
	
}

//--------------------------------------------------------------
void App::jitterAndZoomScene() {
	
	if (cameraXTween.isCompleted()) {
		float x = ofGetScreenWidth()/2+ofRandom(-3, 3);
		float dur = ofRandom(2000, 3000);
		float delay = 0.0;//ofRandom(200, 1000);
		cameraXTween.setParameters(easingelastic, ofxTween::easeInOut, camera.getPosition().x, x, dur, delay);
		eyeXTween.setParameters(easingelastic, ofxTween::easeInOut, camera.getPosition().x, x, dur, delay);

		float y = ofGetScreenHeight()/2+ofRandom(-3, 3);
		delay = 0.0;//ofRandom(200, 1000);		
		cameraYTween.setParameters(easingelastic, ofxTween::easeInOut, camera.getPosition().y, y, dur, delay);
		eyeYTween.setParameters(easingelastic, ofxTween::easeInOut, camera.getPosition().y, y, dur, delay);

	}
	
	if (cameraZTween.isCompleted()) {
		float z = ofRandom(900, 1200);
		float dur = ofRandom(4000, 6000);
		cameraZTween.setParameters(easinglinear, ofxTween::easeInOut, camera.getPosition().z, z, dur, 0);
	}
	
	blobMgr.moveBG();
}

//--------------------------------------------------------------
void App::onFileChangeBG(FileDef& fd) {
	blobMgr.changeImgBG(fd.path);
}
void App::onFileChangeQImg(FileDef& fd) {
	qImage.changeImgQImg(fd.path);	
}
void App::onFileChangeBlobTex(FileDef& fd) {
	blobMgr.changeImgBlobTex(fd.path);
}
void App::onSMSMsgRecieved(UpdateInfo& _upInfo) {
	upInfo = _upInfo;
	sText.onSMSReceivedUpdate(upInfo.sms.YesOrNo, upInfo);
	blobMgr.recieveSMS(upInfo);
}
void App::onClearQImg(int& i) {
	qImage.clear();
}

//--------------------------------------------------------------
int scrnseq = 0;
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
	}else if (key == 'p') {
		scrnseq++;
		ofSaveScreen("/Users/alexbeim/Desktop/akiraTemporary/scrn"+ofToString(scrnseq)+".png");
	}else {	
		adminPanel.keyPressed(key);
	}
}

//--------------------------------------------------------------
void App::keyReleased  (int key){}
//--------------------------------------------------------------
void App::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void App::resized(int w, int h){}