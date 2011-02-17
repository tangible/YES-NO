#include "testApp.h"
#include "ofxSimpleGuiToo.h"

float focus;
float aspectratiox;
float aspectratioy;
float blurclamp;
float bias;

//--------------------------------------------------------------
void testApp::setup(){

	ofSetDataPathRoot("../Resources/");
	ofDisableArbTex();
	ofEnableAlphaBlending();
	ofBackground(255, 255, 255);
	
	cam.setup(this, 700);
	
	defaultShader.setup("default");
	showDepthShader.setup("showdepth");
	dofShader.setup("dof");
	
	depthFBO.setup(ofGetWidth(), ofGetHeight());
	colorFBO.setup(ofGetWidth(), ofGetHeight());
	
	numObj = 100;
	for (int i = 0; i < numObj; i++) {
		float x = ofRandom(ofGetWidth()/2-200, ofGetWidth()/2+200);
		float y = ofRandom(ofGetHeight()/2-200, ofGetHeight()/2+200);
		float z = ofRandom(-500, 700);
		ofxVec3f pos = ofxVec3f(x,y,z);
		float r = ofRandom(0.0, 255.0);
		float g = ofRandom(0.0, 255.0);
		float b = ofRandom(0.0, 255.0);
		float a = ofRandom(200.0, 255.0);
		ofxVec4f col = ofxVec4f(r,g,b,a);
		int size = ofRandom(10, 50);
		objPos.push_back(pos);
		objCol.push_back(col);
		objSize.push_back(size);
	}		
	
	//.setNewColumn(true);
	gui.addTitle("Shader Setting");
	gui.addSlider("focus", focus, 0.0, 2.0);
	gui.addSlider("aspectratiox", aspectratiox, 0.0, ofGetWidth());
	gui.addSlider("aspectratioy", aspectratioy, 0.0, ofGetHeight());
	gui.addSlider("blurclamp", blurclamp, 0.0, 1.0);
	gui.addSlider("bias", bias, 0.0, 1.0);
	gui.loadFromXML();
	gui.show();	
	aspectratiox = ofGetWidth();
	aspectratioy = ofGetHeight();	
	//blurclamp = 3.0;
	//bias = 0.6;
	
}

//--------------------------------------------------------------
void testApp::update(){

	cam.draw();
	
	depthFBO.beforeUpdate();
	drawScene();
	depthFBO.afterUpdate();
	
	colorFBO.beforeUpdate();
	drawScene();
	colorFBO.afterUpdate();	

}

//--------------------------------------------------------------
void testApp::draw(){

//	cam.draw();
//	drawScene();
	
	int colorTexSlot = 4;
	colorFBO.beforeDraw(colorTexSlot);
	int depthTexSlot = 5;
	depthFBO.beforeDraw(depthTexSlot);
	dofShader.begin();
	dofShader.setUniform1i("bgl_RenderedTexture", colorTexSlot);
	dofShader.setUniform1i("bgl_DepthTexture", depthTexSlot);
	dofShader.setUniform1f("focus", focus);
	dofShader.setUniform1f("aspectratiox", aspectratiox);
	dofShader.setUniform1f("aspectratioy", aspectratioy);
	dofShader.setUniform1f("blurclamp", blurclamp);
	dofShader.setUniform1f("bias", bias);
	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
	dofShader.end();
	
//	int colorTexSlot = 4;
//	colorFBO.beforeDraw(colorTexSlot);
//	defaultShader.begin();
//	defaultShader.setUniform1i("tex", colorTexSlot);
//	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
//	defaultShader.end();
//	colorFBO.afterDraw();
	
//	int depthTexSlot = 5;
//	depthFBO.beforeDraw(depthTexSlot);
//	showDepthShader.begin();
//	showDepthShader.setUniform1i("depth", depthTexSlot);
//	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
//	showDepthShader.end();
//	depthFBO.afterDraw();	
	
	
	ofSetupScreen();
	gui.draw();
	
}

//--------------------------------------------------------------
void testApp::drawScene() {
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < numObj; i++) {
		ofxVec3f pos = objPos[i];
		ofxVec4f col = objCol[i];
		int size = objSize[i];
		ofSetColor(col.x, col.y, col.z);
		ofxSphere(pos.x, pos.y, pos.z, size);
//		ofxBox(pos.x, pos.y, pos.z, size);
//		ofPushMatrix();
//		ofTranslate(pos.x, pos.y, pos.z);
//		ofCircle(0, 0, size);
//		ofPopMatrix();
	}

	ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void testApp::drawFullScreenQuad(int w, int h) {
	
	ofxVec3f a = ofxVec3f(0, 0, 0);
	ofxVec3f b = ofxVec3f(ofGetWidth(), 0, 0);
	ofxVec3f c = ofxVec3f(ofGetWidth(), ofGetHeight(), 0);
	ofxVec3f d = ofxVec3f(0, ofGetHeight(), 0);
	
	ofxQuad(a, b, c, d);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key = ' ') {
		gui.toggleDraw();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

