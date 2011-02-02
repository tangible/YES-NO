#include "App.h"

//--------------------------------------------------------------
void App::setup(){
	
	ofSetFrameRate(FPS);
	ofBackground(255,255,255);

	adminPanel.setup();
	blobMgr.setup(FPS, &adminPanel);
	httpClient.setup();
	
	ofAddListener(adminPanel.onFileDialogue, this, &App::onFileChange);
	
}

//--------------------------------------------------------------
void App::update(){
	
	blobMgr.update();
	adminPanel.update();
	httpClient.update();	
	
}

//--------------------------------------------------------------
void App::draw(){
	
	blobMgr.draw();
	adminPanel.draw();
	
}

//--------------------------------------------------------------
void App::onFileChange(FileDef& fd) {
	blobMgr.changeImg(fd.path);
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
		case 'm': 
			vector<MetaBallChunk*> mcs = blobMgr.mBallChunks;
			mcs[0]->minimizeOne();
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