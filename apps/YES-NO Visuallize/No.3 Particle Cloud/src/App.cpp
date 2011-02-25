#include "App.h"

static const int fps = 40;

//--------------------------------------------------------------
void App::setup(){

	ofSetDataPathRoot("../Resources/");
	ofDisableArbTex();
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofBackground(0,0,0);
	ofSetFrameRate(fps);
	
	//cam.setup(this, 700);
	cam.position(ofGetScreenWidth()/2, ofGetScreenHeight()/2, 760);	
	
	adminPanel.setup();
	pCloud.setup(fps, &adminPanel, &cam);
	sText.setup();
	qImage.setup();
	httpClient.setup();
	
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
	
	qImage.changeImgQImg("qimg3.png");

}

//--------------------------------------------------------------
void App::update(){
	
	adminPanel.update();
	pCloud.update();

	cam.place();
	
	ofBackground(adminPanel.BGColor[0]*255, adminPanel.BGColor[1]*255, adminPanel.BGColor[2]*255);
	
	depthFBO.beforeUpdate();
	pCloud.draw();
	depthFBO.afterUpdate();
	
	colorFBO.beforeUpdate();
	pCloud.draw();
	colorFBO.afterUpdate();			
	
	int colorTexSlot = 4;
	colorFBO.beforeDraw(colorTexSlot);	
	int depthTexSlot = 5;
	depthFBO.beforeDraw(depthTexSlot);
	ssaoFBO.beforeUpdate();
	ssaoShader.begin();
	ssaoShader.setUniform1i("texture0", depthTexSlot);
	ssaoShader.setUniform1i("texture1", colorTexSlot);
	ssaoShader.setUniform2f("screensize", ofGetScreenWidth(), ofGetScreenHeight());
	ssaoShader.setUniform2f("camerarange", adminPanel.camerarangex, adminPanel.camerarangey);
	ssaoShader.setUniform1f("aoCap", adminPanel.aoCap);
	ssaoShader.setUniform1f("aoMultiplier", adminPanel.aoMultiplier);
	ssaoShader.setUniform1f("depthTolerance", adminPanel.depthTolerance);
	ssaoShader.setUniform1f("aorange", adminPanel.aorange);
	ssaoShader.setUniform1f("readDepthVal", adminPanel.readDepthVal);
	ssaoShader.setUniform1f("aoDivision", adminPanel.aoDivision);
	ssaoShader.setUniform1f("baseColSubdivision", adminPanel.baseColSubdivision);
	drawFullScreenQuad(ofGetScreenWidth(), ofGetScreenHeight());
	ssaoShader.end();
	ssaoFBO.afterUpdate();
	depthFBO.afterDraw();
	colorFBO.afterDraw();	
	
}

//--------------------------------------------------------------
void App::draw(){
	
	//cam.draw();
	
	bg.draw(ofGetScreenWidth()/2-bg.getWidth()/2, 
			ofGetScreenHeight()/2-bg.getHeight()/2);	
	
	qImage.draw();	
	sText.draw(upInfo);
	
	int ssaoTexSlot = 6;
	ssaoFBO.beforeDraw(ssaoTexSlot);
	int depthTexSlot = 5;
	depthFBO.beforeDraw(depthTexSlot);
	dofShader.begin();
	dofShader.setUniform1i("bgl_RenderedTexture", ssaoTexSlot);
	dofShader.setUniform1i("bgl_DepthTexture", depthTexSlot);
	dofShader.setUniform1f("focus", adminPanel.focus);
	dofShader.setUniform1f("aspectratiox", adminPanel.aspectratiox);
	dofShader.setUniform1f("aspectratioy", adminPanel.aspectratioy);
	dofShader.setUniform1f("blurclamp", adminPanel.blurclamp);
	dofShader.setUniform1f("bias", adminPanel.bias);
	drawFullScreenQuad(ofGetScreenWidth(), ofGetScreenHeight());
	dofShader.end();	

//	int colorTexSlot = 4;
//	colorFBO.beforeDraw(colorTexSlot);
//	defaultShader.begin();
//	defaultShader.setUniform1i("tex", colorTexSlot);
//	drawFullScreenQuad(ofGetScreenWidth(), ofGetScreenHeight());
//	defaultShader.end();
//	colorFBO.afterDraw();
	
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
	}else {
		adminPanel.keyPressed(key);	
		pCloud.debugKeyPress(key);	
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
	pCloud.feedSMS(upInfo);
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

