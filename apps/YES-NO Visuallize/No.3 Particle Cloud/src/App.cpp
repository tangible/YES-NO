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
	
	adminPanel.setup();
	pCloud.setup(fps, &adminPanel);
	
	//cam.setup(this, 700);
	
	defaultShader.setup("default");
	showDepthShader.setup("showdepth");
	ssaoShader.setup("ssao");	
	dofShader.setup("dof");
	
	depthFBO.setup(ofGetWidth(), ofGetHeight());
	colorFBO.setup(ofGetWidth(), ofGetHeight());
	ssaoFBO.setup(ofGetWidth(), ofGetHeight());	
	
}

//--------------------------------------------------------------
void App::update(){
	
	adminPanel.update();
	pCloud.update();

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
	ssaoShader.setUniform2f("screensize", ofGetWidth(), ofGetHeight());
	ssaoShader.setUniform2f("camerarange", adminPanel.camerarangex, adminPanel.camerarangey);
	ssaoShader.setUniform1f("aoCap", adminPanel.aoCap);
	ssaoShader.setUniform1f("aoMultiplier", adminPanel.aoMultiplier);
	ssaoShader.setUniform1f("depthTolerance", adminPanel.depthTolerance);
	ssaoShader.setUniform1f("aorange", adminPanel.aorange);
	ssaoShader.setUniform1f("readDepthVal", adminPanel.readDepthVal);
	ssaoShader.setUniform1f("aoDivision", adminPanel.aoDivision);
	ssaoShader.setUniform1f("baseColSubdivision", adminPanel.baseColSubdivision);
	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
	ssaoShader.end();
	ssaoFBO.afterUpdate();
	depthFBO.afterDraw();
	colorFBO.afterDraw();	
	
}

//--------------------------------------------------------------
void App::draw(){
	
	//cam.draw();
	
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
	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
	dofShader.end();	

	adminPanel.draw();
	
}

//--------------------------------------------------------------
void App::drawFullScreenQuad(int w, int h) {
	
	ofxVec3f a = ofxVec3f(0, 0, 0);
	ofxVec3f b = ofxVec3f(ofGetWidth(), 0, 0);
	ofxVec3f c = ofxVec3f(ofGetWidth(), ofGetHeight(), 0);
	ofxVec3f d = ofxVec3f(0, ofGetHeight(), 0);
	ofxQuad(a, b, c, d);
	
}

//--------------------------------------------------------------
void App::keyPressed(int key){

	if (key = ' ') 
		adminPanel.toggle();

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

