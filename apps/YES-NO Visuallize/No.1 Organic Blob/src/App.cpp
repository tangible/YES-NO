#include "App.h"

//--------------------------------------------------------------
void App::setup(){
	
	ofSetFrameRate(FPS);
	ofBackground(255,255,255);
	ofSetDataPathRoot("../Resources/");

	adminPanel.setup();
	qImage.setup();
	stateText.setup();
	blobMgr.setup(FPS, &adminPanel, &qImage, &stateText);
	httpClient.setup();
	
	ofAddListener(adminPanel.onFileDialogueBG, this, &App::onFileChangeBG);
	ofAddListener(adminPanel.onFileDialogueQImg, this, &App::onFileChangeQImg);	
	ofAddListener(adminPanel.onFileDialogueBlobTex, this, &App::onFileChangeBlobTex);	
	ofAddListener(httpClient.onSMSRecieved, this, &App::onSMSMsgRecieved);
	
	camera.position(ofGetWidth()/2, ofGetHeight()/2, 700);
	cameraXTween.setParameters(easingback, ofxTween::easeOut, ofGetWidth()/2, ofGetWidth()/2, 0, 0);
	eyeXTween.setParameters(easingback, ofxTween::easeOut, ofGetWidth()/2, ofGetWidth()/2, 0, 0);
	cameraYTween.setParameters(easingback, ofxTween::easeOut, ofGetHeight()/2, ofGetHeight()/2, 0, 0);
	eyeYTween.setParameters(easingback, ofxTween::easeOut, ofGetHeight()/2, ofGetHeight()/2, 0, 0);
	cameraZTween.setParameters(easingback, ofxTween::easeOut, 700, 700, 0, 0);
	
}

//--------------------------------------------------------------
void App::update(){
	
	blobMgr.update();
	adminPanel.update();
	httpClient.update();	
	
	jitterAndZoomScene();
	camera.position(cameraXTween.update(), cameraYTween.update(), cameraZTween.update());
	camera.eye(eyeXTween.update(), eyeYTween.update(), 0);
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
		float x = ofGetWidth()/2+ofRandom(-3, 3);
		float dur = ofRandom(2000, 3000);
		float delay = 0.0;//ofRandom(200, 1000);
		cameraXTween.setParameters(easingelastic, ofxTween::easeInOut, camera.getPosition().x, x, dur, delay);
		eyeXTween.setParameters(easingelastic, ofxTween::easeInOut, camera.getPosition().x, x, dur, delay);

		float y = ofGetHeight()/2+ofRandom(-3, 3);
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

//--------------------------------------------------------------
void App::onFileChangeQImg(FileDef& fd) {
	qImage.changeImgQImg(fd.path);	
}

//--------------------------------------------------------------
void App::onFileChangeBlobTex(FileDef& fd) {
	blobMgr.changeImgBlobTex(fd.path);
}

//--------------------------------------------------------------
void App::onSMSMsgRecieved(UpdateInfo& upInfo) {
	blobMgr.recieveSMS(upInfo);
}

//--------------------------------------------------------------
void App::keyPressed  (int key){
//    switch ((char)key){
//		case ' ':
//			ofxVec3f rdmPos = ofxVec3f(ofGetWidth()/2+ofRandom(-500, 500), ofGetHeight()/2+ofRandom(-500, 500), 600);
//			
//			MyRigidBody* sph = bullet->createSphere(rdmPos,
//													ofRandom(50, 70), 
//													1, 
//													ofxVec4f(ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), 0.7), 
//													DYNAMIC_BODY);		
//			
//			int rdmIdx = ofRandom(0, spheres.size()-1);
//			MyRigidBody* r = spheres[rdmIdx];
//			r->remove(bullet->getWorld());
//			spheres.erase(spheres.begin()+rdmIdx);		
//			spheres.push_back(sph);	
//			break;
//    }
	
	switch(key) {
		case ' ': adminPanel.toggle(); break;
		case 's': 
			httpClient.sendRequest();
			break;	
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