#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetDataPathRoot("../Resources/");
	ofDisableArbTex();
	ofEnableAlphaBlending();
	
	defaultShader.setup("default");
	showDepthShader.setup("showdepth");
	ssaoShader.setup("ssao");
	
	sceneColFBO.setup(ofGetWidth(), ofGetHeight());	
	depthFBO.setup(ofGetWidth(), ofGetHeight());
	sceneFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);	
	
	numObj = 100;
	for (int i = 0; i < numObj; i++) {
		float x = ofRandom(ofGetWidth()/2-200, ofGetWidth()/2+200);
		float y = ofRandom(ofGetHeight()/2-200, ofGetHeight()/2+200);
		float z = ofRandom(-200, 200);
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
}

//--------------------------------------------------------------
void testApp::update(){
	
	sceneFBO.begin();
	drawScene();
	sceneFBO.end();
	
	sceneColFBO.beforeUpdate();
	drawScene();
	sceneColFBO.afterUpdate();
	
	depthFBO.beforeUpdate();
	drawScene();
	depthFBO.afterUpdate();
	
}

//--------------------------------------------------------------
void testApp::draw(){

//	sceneFBO.draw(0, 0);	
	
	
//	glActiveTexture(GL_TEXTURE2);
//	glBindBuffer(GL_TEXTURE_RECTANGLE_ARB, (GLuint)sceneFBO.texData.textureID);
//	sceneFBO.draw(0, 0, ofGetWidth(), ofGetHeight());			
//	defaultShader.begin();
//	defaultShader.setUniform1i("tex", 2);
//	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
//	defaultShader.end();
	
//	int sceneColSlot = 6;
//	sceneColFBO.beforeDraw(sceneColSlot);
//	defaultShader.begin();
//	defaultShader.setUniform1i("tex", sceneColSlot);
//	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
//	defaultShader.end();
//	sceneColFBO.afterDraw();	
	
	int depthTexSlot = 5;
	depthFBO.beforeDraw(depthTexSlot);
	showDepthShader.begin();
	showDepthShader.setUniform1i("depth", depthTexSlot);
	drawFullScreenQuad(ofGetWidth(), ofGetHeight());
	showDepthShader.end();
	depthFBO.afterDraw();
	
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

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(0, h);
	glVertex2f(0, h);
	glTexCoord2f(w, 0);
	glVertex2f(w, 0);
	glTexCoord2f(w, h);
	glVertex2f(w, h);
	glEnd();	
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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

