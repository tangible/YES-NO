#include "App.h"

//--------------------------------------------------------------
void App::setup(){
	
	ofSetFrameRate(FPS);
	ofBackground(0,0,0);
	ofSetDataPathRoot("../Resources/");
	ofSetVerticalSync(true);

	adminPanel.setup();
	qImage.setup();
	sText.setup();
	blobMgr.setup(FPS, &adminPanel, &camera, &qImage, &sText);
	httpClient.setup(&adminPanel);
	
	ofAddListener(adminPanel.onFileDialogueBG, this, &App::onFileChangeBG);
	ofAddListener(adminPanel.onFileDialogueQImg, this, &App::onFileChangeQImg);	
	ofAddListener(adminPanel.onFileDialogueBlobTex, this, &App::onFileChangeBlobTex);	
	ofAddListener(httpClient.onSMSRecieved, this, &App::onSMSMsgRecieved);
	ofAddListener(adminPanel.onClearQImg, this, &App::onClearQImg);		
	ofAddListener(adminPanel.onRestoreAllSMSAnswer, this, &App::onRestoreAllSMSAnswer);	
	bAlreadyRestoreAllAnswer = false;	
	
	camera.position(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 1060);
	cameraXTween.setParameters(easingback, ofxTween::easeOut, ofGetScreenWidth()/2, ofGetScreenWidth()/2, 0, 0);
	eyeXTween.setParameters(easingback, ofxTween::easeOut, ofGetScreenWidth()/2, ofGetScreenWidth()/2, 0, 0);
	cameraYTween.setParameters(easingback, ofxTween::easeOut, ofGetScreenHeight()/2, ofGetScreenHeight()/2, 0, 0);
	eyeYTween.setParameters(easingback, ofxTween::easeOut, ofGetScreenHeight()/2, ofGetScreenHeight()/2, 0, 0);
	cameraZTween.setParameters(easingback, ofxTween::easeOut, 700, 700, 0, 0);
	
	//qImage.changeImgQImg("qimg3.png");
	
	bool bCheckSetting = adminPanel.checkSetting();
	if (bCheckSetting) {
		int i = 1;
		ofNotifyEvent(adminPanel.onRestoreAllSMSAnswer, i);	
	}
}

//--------------------------------------------------------------
void App::update(){
	
	blobMgr.update();
	adminPanel.update();
	httpClient.update(adminPanel.debugWithFakeSMS);
	
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
void App::onRestoreAllSMSAnswer(int& i) {
	if (!bAlreadyRestoreAllAnswer)
		httpClient.sendRequestToServer(true);
	bAlreadyRestoreAllAnswer = true;
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
	}else if (key == 'y') {
		httpClient.emulateSMS(0);
	}else if (key == 'u') {
		for (int i = 0; i < 10; i++) {
			httpClient.emulateSMS(0);
		}
	}else if (key == 'i') {
		for (int i = 0; i < 100; i++) {		
			httpClient.emulateSMS(0);	
		}
	}else if (key == 'p') {
		scrnseq++;
		ofSaveScreen("/Users/alexbeim/Desktop/akiraTemporary/scrn"+ofToString(scrnseq)+".png");
	}else if (key == 'f') {
		httpClient.createFakeSMS();
	}else if (key == 'x') {
		float rnd = ofRandomf();		
		cout << "save to xml file = "+ofToString(rnd) << endl;
		xmlSetting.saveToXMLFile("tag", "value"+ofToString(rnd));
	}else if (key == 'z') {
		cout << "extract from file" << endl;
		string rtn = xmlSetting.getValueByTag("tag", "error");
		cout << "returned = "+rtn << endl;
	}else if (key == 'l') {
		sText.setup();
		//blobMgr.reset();
		blobMgr.setup(FPS, &adminPanel, &camera, &qImage, &sText);		
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