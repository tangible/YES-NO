#include "testApp.h"
#include "ofxSimpleGuiToo.h"

//--------------------------------------------------------------
float camerarangex;
float camerarangey;
float aoCap;
float aoMultiplier;
float depthTolerance;
float aorange;
void testApp::setup(){

	ofSetDataPathRoot("../Resources/");
	ofDisableArbTex();
	ofEnableAlphaBlending();
	
	defaultShader.setup("default");
	showDepthShader.setup("showdepth");
	ssaoShader.setup("ssao");
	
	depthFBO.setup(ofGetWidth(), ofGetHeight());
	sceneFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);	
	
	numObj = 100;
	for (int i = 0; i < numObj; i++) {
		float x = ofRandom(ofGetWidth()/2-200, ofGetWidth()/2+200);
		float y = ofRandom(ofGetHeight()/2-200, ofGetHeight()/2+200);
		float z = ofRandom(-1000, 1000);
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
	gui.addSlider("camerarangex", camerarangex, 0, 10000);
	gui.addSlider("camerarangey", camerarangey, 0, 10000);
	gui.addSlider("aoCap", aoCap, 0.0, 2.0);
	gui.addSlider("aoMultiplier", aoMultiplier, 0.0, 20000.0);
	gui.addSlider("depthTolerance", depthTolerance, 0.000, 0.3);
	gui.addSlider("aorange", aorange, 0.0, 20.0);
	gui.loadFromXML();
	gui.show();		
}

//--------------------------------------------------------------
void testApp::update(){
	
	sceneFBO.begin();
	drawScene();
	sceneFBO.end();
	
	depthFBO.beforeUpdate();
	drawScene();
	depthFBO.afterUpdate();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	glActiveTexture(GL_TEXTURE2);
	glBindBuffer(GL_TEXTURE_RECTANGLE_ARB, (GLuint)sceneFBO.texData.textureID);
	sceneFBO.draw(0, 0, ofGetWidth(), ofGetHeight());	
	int depthTexSlot = 5;
	depthFBO.beforeDraw(depthTexSlot);
	ssaoShader.begin();
	ssaoShader.setUniform1i("texture0", 5);
	ssaoShader.setUniform1i("texture1", 2);
	ssaoShader.setUniform2f("screensize", ofGetWidth(), ofGetHeight());
	
	//--
	ssaoShader.setUniform2f("camerarange", camerarangex, camerarangey);
	ssaoShader.setUniform1f("aoCap", aoCap);
	ssaoShader.setUniform1f("aoMultiplier", aoMultiplier);
	ssaoShader.setUniform1f("depthTolerance", depthTolerance);
	ssaoShader.setUniform1f("aorange", aorange);
	//--
	
	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
	ssaoShader.end();
	

//	sceneFBO.draw(0, 0);	
	
//	glActiveTexture(GL_TEXTURE2);
//	glBindBuffer(GL_TEXTURE_2D, (GLuint)sceneFBO.texData.textureID);
//	sceneFBO.draw(0, 0, ofGetWidth(), ofGetHeight());			
//	defaultShader.begin();
//	defaultShader.setUniform1i("tex", 2);
//	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
//	defaultShader.end();
	
//	int depthTexSlot = 5;
//	depthFBO.beforeDraw(depthTexSlot);
//	showDepthShader.begin();
//	showDepthShader.setUniform1i("depth", depthTexSlot);
//	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
//	showDepthShader.end();
//	depthFBO.afterDraw();
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::drawScene() {
	
	ofPushMatrix();
	for (int i = 0; i < numObj; i++) {
		ofxVec3f pos = objPos[i];
		ofxVec4f col = objCol[i];
		int size = objSize[i];
		ofSetColor(col.x, col.y, col.z);
		ofxSphere(pos.x, pos.y, pos.z, size);
	}
	ofPopMatrix();
	ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void testApp::drawFullScreenQuad(int w, int h) {

	ofxVec3f a = ofxVec3f(0, 0, 0);
	ofxVec3f b = ofxVec3f(ofGetWidth(), 0, 0);
	ofxVec3f c = ofxVec3f(ofGetWidth(), ofGetHeight(), 0);
	ofxVec3f d = ofxVec3f(0, ofGetHeight(), 0);

	ofxQuad(a, b, c, d);
	
//	glBegin(GL_TRIANGLE_STRIP);
//	glTexCoord2f(0, 0);
//	glVertex2f(0, 0);
//	glTexCoord2f(0, h);
//	glVertex2f(0, h);
//	glTexCoord2f(w, 0);
//	glVertex2f(w, 0);
//	glTexCoord2f(w, h);
//	glVertex2f(w, h);
//	glEnd();	
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	if (key = ' ') {
		gui.toggleDraw();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

