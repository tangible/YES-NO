#include "App.h"

const int fps = 40;

//--------------------------------------------------------------
void App::setup(){

	bgPlayer = new ofVideoPlayer();		
	
	ofSetFrameRate(fps);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetDataPathRoot("../Resources/");
	ofBackground(0,0,0);
		
	cam.position(ofGetWidth()/2, ofGetWidth()/2, 1000);
	//cam.setup(this, 1000);
	adminPanel.setup();
	sText.setup();	
	convexHull.setup(fps, &adminPanel, &cam);
	qImage.setup();
	httpClient.setup(&adminPanel);
	
	ofAddListener(adminPanel.onFileDialogueBG, this, &App::onFileChangeBG);	
	ofAddListener(adminPanel.onClearBG, this, &App::onClearBG);	
	ofAddListener(adminPanel.onFileDialogueQImg, this, &App::onFileChangeQImg);	
	ofAddListener(adminPanel.onClearQImg, this, &App::onClearQImg);		
	ofAddListener(adminPanel.onRestoreAllSMSAnswer, this, &App::onRestoreAllSMSAnswer);
	ofAddListener(httpClient.onSMSRecieved, this, &App::onSMSMsgRecieved);	
	ofAddListener(convexHull.yesSoft.onFinishAllUpdating, this, &App::resotoreCamOrbit);	
	ofAddListener(convexHull.noSoft.onFinishAllUpdating, this, &App::resotoreCamOrbit);
	ofAddListener(convexHull.yesSoft.notifyStartCamOrbit, this, &App::camOrbit);
	ofAddListener(convexHull.noSoft.notifyStartCamOrbit, this, &App::camOrbit);	
	bAlreadyRestoreAllAnswer = false;	
	ofAddListener(convexHull.yesSoft.notifyUpdateStextColorYesEvent, this, &App::onNotifyUpdateStextColorYesEvent);
	ofAddListener(convexHull.noSoft.notifyUpdateStextColorNoEvent, this, &App::onNotifyUpdateStextColorNoEvent);
	ofAddListener(convexHull.yesSoft.notifyStartStextFadingYesEvent, this, &App::onNotifyStartStextFadingYesEvent);
	ofAddListener(convexHull.noSoft.notifyStartStextFadingNoEvent, this, &App::onNotifyStartStextFadingNoEvent);	
	
	//qImage.changeImgQImg("qimg3.png");
	prevOrbit = 2;
	camOrbitAmt = 4.5;
	
	bool bCheckSetting = adminPanel.checkSetting();
	if (bCheckSetting) {
		int i = 1;
		ofNotifyEvent(adminPanel.onRestoreAllSMSAnswer, i);	
	}	

	bgenY = false;
}

//--------------------------------------------------------------
void App::update(){
	
	adminPanel.update();
	convexHull.update();
	httpClient.update(adminPanel.debugWithFakeSMS);
	
	if (isVidBG) {
		bgPlayer->update();
		if (bgPlayer->bLoaded && !bgPlayer->isFrameNew()) bgPlayer->play();			
	}		
	
	cam.orbitAround(cam.getEye(), ofxVec3f(0,1,0), camOrbitTween.update());
	
	if (!convexHull.isYesUpdating && !convexHull.isNoUpdating && smsQue.size() > 0) {
		
		upInfo = smsQue[0];
		smsQue.erase(smsQue.begin());
		
		sText.onSMSReceivedUpdate(upInfo.sms.YesOrNo, upInfo);
		convexHull.feedSMS(upInfo);
		
//		if (prevOrbit != upInfo.sms.YesOrNo) {
//			float oamt = 0.0;
//			if (upInfo.sms.YesOrNo == 0) {
//				oamt = -camOrbitAmt;
//			}else {
//				oamt = camOrbitAmt;
//			}
//			if (prevOrbit != 2) oamt *= 2;
//			camOrbitTween.setParameters(camOrbitEasing, ofxTween::easeInOut, oamt, 0.0, 600, 0);
//			
//		}
//		prevOrbit = upInfo.sms.YesOrNo;	
	}
	
	int tgtn = 5000;
	if (bgenY) {
		int num = convexHull.yesSoft.genShapeProgramatically();
		if (tgtn <= num) {
			bgenY = false;
		}
	}
}

void App::camOrbit(int & z) {
	
	
	if (prevOrbit != upInfo.sms.YesOrNo) {
		float oamt = 0.0;
		if (upInfo.sms.YesOrNo == 0) {
			oamt = -camOrbitAmt;
		}else {
			oamt = camOrbitAmt;
		}
		if (prevOrbit != 2) oamt *= 2;
		camOrbitTween.setParameters(camOrbitEasing, ofxTween::easeInOut, oamt, 0.0, 600, 0);
		
	}
	prevOrbit = upInfo.sms.YesOrNo;		
	
}

void App::resotoreCamOrbit(int & z) {
	
	if (smsQue.size() == 0) {
		float oamt = 0.0;
		if (prevOrbit == 0) { // yes
			oamt = camOrbitAmt;
		}else if (prevOrbit == 1) { // no
			oamt = -camOrbitAmt;
		}
		camOrbitTween.setParameters(camOrbitEasing, ofxTween::easeInOut, oamt, 0.0, 600, 0);
		prevOrbit = 2;	
	}
}

//--------------------------------------------------------------
void App::draw(){
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisableClientState(GL_COLOR_ARRAY);
	ofEnableAlphaBlending();
	ofEnableSmoothing();	
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
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	ofPopMatrix();
	
	
	cam.place();
	//cam.draw();
	convexHull.draw();		

	adminPanel.draw();	
	ofSetColor(255, 255, 255);	
	
}

//--------------------------------------------------------------
int scrnseq = 0;
int atonceidx = 0;
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
	}else if (key == 'y') {
		int z = 0;
		for (int i = 0; i < 1; i++) {
			ofxVec3f cen = convexHull.yesSoft.yesORno->getBodyCentroid();
			vector<int> sortedFaces = convexHull.yesSoft.yesORno->sortFaceByPosition(cen);
//			int faceID = sortedFaces[sortedFaces.size()-1];
//			convexHull.yesSoft.addSMS(faceID, 1, 1);			
			int faceID = sortedFaces[ofRandom(0, sortedFaces.size()-1*0.3)];
			convexHull.yesSoft.addSMS(faceID, 1, 1);
			convexHull.yesSoft.addSMSCompleted(i);
		}
	}else if (key == 'n') {
		
	}else if (key == 'r') {
		httpClient.sendRequestToServer(false, true);
	}else if (key == 'f') {
		httpClient.createFakeSMS();
	}else if (key == 'g') {
//		convexHull.yesSoft.genShapeAtOnce(10);
//		atonceidx++;
		bgenY = true;
		cout << "atonceidx = " + ofToString(atonceidx) << endl;
	}else {	
		adminPanel.keyPressed(key);
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
	smsQue.push_back(_upInfo);
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

