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
		
	cam.position(ofGetWidth()/2, ofGetWidth()/2-400, 1000);
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
	ofAddListener(httpClient.onAllSMSRecieved, this, &App::onAllSMSMsgRecieved);
	ofAddListener(convexHull.yesSoft.onFinishAllUpdating, this, &App::resotoreCamOrbit);	
	ofAddListener(convexHull.noSoft.onFinishAllUpdating, this, &App::resotoreCamOrbit);
	ofAddListener(convexHull.yesSoft.notifyStartCamOrbit, this, &App::camOrbit);
	ofAddListener(convexHull.noSoft.notifyStartCamOrbit, this, &App::camOrbit);	
	bAlreadyRestoreAllAnswer = false;	
	ofAddListener(convexHull.yesSoft.notifyUpdateStextColorYesEvent, this, &App::onNotifyUpdateStextColorYesEvent);
	ofAddListener(convexHull.noSoft.notifyUpdateStextColorNoEvent, this, &App::onNotifyUpdateStextColorNoEvent);
	ofAddListener(convexHull.yesSoft.notifyStartStextFadingYesEvent, this, &App::onNotifyStartStextFadingYesEvent);
	ofAddListener(convexHull.noSoft.notifyStartStextFadingNoEvent, this, &App::onNotifyStartStextFadingNoEvent);	
	
	prevOrbit = 2;
	camOrbitAmt = 6.5;
	camOrbitDur = 600;
//	camOrbitAxisY = ofxVec3f(-0.3,1.3,0);
//	camOrbitAxisN = ofxVec3f(0.2,0.8,0);
	camOrbitAxisY = ofxVec3f(0.0,1,0);
	camOrbitAxisN = ofxVec3f(0.0,1,0);	
	
	bCheckSetting = adminPanel.checkSetting();
	if (bCheckSetting) {
		httpClient.sendRequestToServer(true, false, true);
	}
	processAllSMS = false;

	generatedUpInfo.numTotalYes = 0;
	generatedUpInfo.numTotalNo = 0;
}


void App::camOrbit(int & z) {
	
	
	if (prevOrbit != upInfo.sms.YesOrNo) {
		float oamt = 0.0;
		if (upInfo.sms.YesOrNo == 0) {
			oamt = -camOrbitAmt;
			//camOrbitAxisY.x = ofRandom(-0.1, 0.0);
			camOrbitAxis = camOrbitAxisY;
		}else {
			oamt = camOrbitAmt;
			//camOrbitAxisN.x = ofRandom(0.0, 0.11);			
			camOrbitAxis = camOrbitAxisN;			
		}
		if (prevOrbit != 2) oamt *= 2;
		camOrbitTween.setParameters(camOrbitEasing, ofxTween::easeInOut, oamt, 0.0, camOrbitDur, 0);
		
	}
	prevOrbit = upInfo.sms.YesOrNo;		
	
}

void App::resotoreCamOrbit(int & z) {
	
	if (smsQue.size() == 0) {
		float oamt = 0.0;
		if (prevOrbit == 0) { // yes
			oamt = camOrbitAmt;
			//camOrbitAxis = camOrbitAxisY;			
		}else if (prevOrbit == 1) { // no
			oamt = -camOrbitAmt;
			//camOrbitAxis = camOrbitAxisN;			
		}
		camOrbitTween.setParameters(camOrbitEasing, ofxTween::easeInOut, oamt, 0.0, camOrbitDur, 0);
		prevOrbit = 2;	
	}
}

//--------------------------------------------------------------
int done = 0;
void App::update(){
	
	adminPanel.update();
	convexHull.update();
	httpClient.update(adminPanel.debugWithFakeSMS);
	
	if (isVidBG) {
		bgPlayer->update();
		if (bgPlayer->bLoaded && !bgPlayer->isFrameNew()) bgPlayer->play();			
	}		
	
	cam.qorbitAround(cam.getEye(), camOrbitAxis, camOrbitTween.update());
	
	if (!convexHull.isYesUpdating && !convexHull.isNoUpdating && smsQue.size() > 0 && !bCheckSetting) {
		
		upInfo = smsQue[0];
		smsQue.erase(smsQue.begin());
		
		sText.onSMSReceivedUpdate(upInfo.sms.YesOrNo, upInfo);
		convexHull.feedSMS(upInfo);

	}
	
	if (processAllSMS) {
		upInfo = allSmsQue[0];
		if (upInfo.sms.YesOrNo == 0 && genYesNum == 1) {
			upInfo = allSmsQue[0];
			allSmsQue.erase(allSmsQue.begin());
			smsQue.push_back(upInfo);
			genYesNum--;
		}else if (upInfo.sms.YesOrNo == 1 && genNoNum == 1) {
			upInfo = allSmsQue[0];
			allSmsQue.erase(allSmsQue.begin());
			smsQue.push_back(upInfo);
			genNoNum--;
		}else if (genNoNum == 0 && genYesNum == 0) {
			processAllSMS = false;
			bCheckSetting = false;
		}else {
			upInfo = allSmsQue[0];
			allSmsQue.erase(allSmsQue.begin());
			
			sText.onSMSReceivedUpdate(upInfo.sms.YesOrNo, upInfo);
			if (upInfo.sms.YesOrNo == 0) {
				int num = convexHull.yesSoft.genShapeProgramatically();
				genYesNum--;
			}else {
				int num = convexHull.noSoft.genShapeProgramatically();
				genNoNum--;
			}			
		}
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

//	glDisable(GL_LIGHTING);		
//	ofDrawBitmapString(ofToString(prevOrbit), 10, 20);
}

//--------------------------------------------------------------
int scrnseq = 0;
int atonceidx = 0;
int atonceidxn = 0;
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
		convexHull.yesSoft.genShapeProgramatically();
		atonceidx++;
		cout << "y="+ofToString(atonceidx) << endl;
	}else if (key == 'h') {
		convexHull.noSoft.genShapeProgramatically();		
		atonceidxn++;		
		cout << "n="+ofToString(atonceidxn) << endl;		
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
	smsQue.push_back(upInfo);
}
void App::onRestoreAllSMSAnswer(int& i) {
	if (!bAlreadyRestoreAllAnswer) {
		bCheckSetting = true;
		httpClient.sendRequestToServer(true, false, true);
		processAllSMS = false;
	}
	bAlreadyRestoreAllAnswer = true;	
}
void App::onAllSMSMsgRecieved(vector<UpdateInfo>& _upInfos) {
	
	processAllSMS = true;
	allSmsQue = _upInfos;
	
//	genYesNum
	for (int i = 0; i < allSmsQue.size(); i++) {
		upInfo = allSmsQue[i];
		if (upInfo.sms.YesOrNo == 0) {
			genYesNum++;
		}else {
			genNoNum++;
		}
	}
	
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

