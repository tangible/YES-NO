#include "App.h"

static const int fps = 25;

//--------------------------------------------------------------
void App::setup(){

	bgPlayer = new ofVideoPlayer();	
	
	ofSetDataPathRoot("../Resources/");
	ofDisableArbTex();
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofBackground(0,0,0);
	ofSetFrameRate(fps);
	ofSetVerticalSync(true);	
	
	//cam.setup(this, 700);
	cam.position(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 760);	
	
	adminPanel.setup();
	pCloud.setup(fps, &adminPanel, &cam);
	sText.setup();
	qImage.setup();
	httpClient.setup(&adminPanel);
	
	defaultShader.setup("default");
	showDepthShader.setup("showdepth");
	ssaoShader.setup("ssao");	
	dofShader.setup("dof");
	
	depthFBO.setup(ofGetScreenWidth(), ofGetScreenHeight());
	colorFBO.setup(ofGetScreenWidth(), ofGetScreenHeight());
	ssaoFBO.setup(ofGetScreenWidth(), ofGetScreenHeight());	
	
	ofAddListener(adminPanel.onFileDialogueBG, this, &App::onFileChangeBG);	
	ofAddListener(adminPanel.onClearBG, this, &App::onClearBG);	
	ofAddListener(adminPanel.onFileDialogueQImg, this, &App::onFileChangeQImg);	
	ofAddListener(adminPanel.onClearQImg, this, &App::onClearQImg);		
	ofAddListener(httpClient.onSMSRecieved, this, &App::onSMSMsgRecieved);	
	ofAddListener(adminPanel.onRestoreAllSMSAnswer, this, &App::onRestoreAllSMSAnswer);	
	bAlreadyRestoreAllAnswer = false;
	ofAddListener(pCloud.yes.notifyUpdateStextColorYesEvent, this, &App::onNotifyUpdateStextColorYesEvent);
	ofAddListener(pCloud.no.notifyUpdateStextColorNoEvent, this, &App::onNotifyUpdateStextColorNoEvent);
	ofAddListener(pCloud.yes.notifyStartStextFadingYesEvent, this, &App::onNotifyStartStextFadingYesEvent);
	ofAddListener(pCloud.no.notifyStartStextFadingNoEvent, this, &App::onNotifyStartStextFadingNoEvent);	
	
	bool bCheckSetting = adminPanel.checkSetting();
	if (bCheckSetting) {
		int i = 1;
		ofNotifyEvent(adminPanel.onRestoreAllSMSAnswer, i);	
	}	
	
}

//--------------------------------------------------------------
void App::update(){
	
	adminPanel.update();
	httpClient.update(adminPanel.debugWithFakeSMS);	
	pCloud.update();
	
	if (isVidBG) {
		bgPlayer->update();
		if (bgPlayer->bLoaded && !bgPlayer->isFrameNew()) bgPlayer->play();			
	}	
	
	cam.place();
	
	ofBackground(adminPanel.BGColor[0]*255, adminPanel.BGColor[1]*255, adminPanel.BGColor[2]*255);
	
}

//--------------------------------------------------------------
void App::draw(){
	
	//cam.draw();
	glDisable(GL_DEPTH_TEST);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	if (isVidBG) {
		bgPlayer->draw(ofGetScreenWidth()/2-bgPlayer->getWidth()/2, 
					   ofGetScreenHeight()/2-bgPlayer->getHeight()/2);
	}else {
		bg.draw(ofGetScreenWidth()/2-bg.getWidth()/2, 
				ofGetScreenHeight()/2-bg.getHeight()/2);		
	}
	
	ofPushMatrix();
	ofSetupScreen();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);		
	ofEnableSmoothing(); 	
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);
	qImage.draw();
	glDisable(GL_CULL_FACE);	
	ofEnableSmoothing(); 
	sText.drawWithNoScale(upInfo);	
	glEnable(GL_LIGHTING);
	//	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	ofPopMatrix();
	ofSetColor(255, 255, 255);			
	
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	pCloud.draw();	

	
	adminPanel.draw();
	
}

//--------------------------------------------------------------
void App::drawFullScreenQuad(int w, int h) {
	
	ofxVec3f a = ofxVec3f(0, 0, 0);
	ofxVec3f b = ofxVec3f(ofGetScreenWidth(), 0, 0);
	ofxVec3f c = ofxVec3f(ofGetScreenWidth(), ofGetScreenHeight(), 0);
	ofxVec3f d = ofxVec3f(0, ofGetScreenHeight(), 0);
	ofxQuad(a, b, c, d);
	
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
	}else {
		adminPanel.keyPressed(key);	
		pCloud.debugKeyPress(key);	
	}
}

//--------------------------------------------------------------
void App::onFileChangeBG(FileDef& fd) {
	changeImgBG(fd.path);
}
void App::onClearBG(int& i) {
	if (isVidBG) {
		bgPlayer->stop();
		bgPlayer->close();
		delete bgPlayer;
		bgPlayer = new ofVideoPlayer();
		isVidBG = false;
		bg.clear();
	}else {
		bg.clear();
	}
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
	pCloud.feedSMS(upInfo);
}
void App::onRestoreAllSMSAnswer(int& i) {
	if (!bAlreadyRestoreAllAnswer)
		httpClient.sendRequestToServer(true);
	bAlreadyRestoreAllAnswer = true;
}

//--------------------------------------------------------------
void App::changeImgBG(string path) {
	
	bool isImg = bg.loadImage(path);
	
	if (isImg) {			
		isVidBG = false;
		bgPlayer->stop();
		bgPlayer->close();
		bgPlayer->stop();
		bgPlayer->close();
		delete bgPlayer;
		bgPlayer = new ofVideoPlayer();		
		
	}else {
		bgPlayer->stop();
		bgPlayer->close();
		delete bgPlayer;
		bgPlayer = new ofVideoPlayer();		
		bgPlayer->loadMovie(path);
		bgPlayer->play();		
		isVidBG = true;
		
	}
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

