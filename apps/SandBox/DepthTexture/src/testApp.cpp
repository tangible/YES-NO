#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	ofSetDataPathRoot("../Resources/");
//	ofDisableArbTex();
	
	showDepthShader.setup("showdepth");
	
	depthFBO.setup(ofGetWidth(), ofGetHeight());
	
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

	depthFBO.beforeUpdate();
	drawScene();
	depthFBO.afterUpdate();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	int depthTexSlot = 5;
	depthFBO.beforeDraw(depthTexSlot);
	showDepthShader.begin();
	showDepthShader.setUniform1i("depth", depthTexSlot);
	drawFullScreenQuad();
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
}

//--------------------------------------------------------------
void testApp::drawFullScreenQuad() {
	
	static const GLfloat quad[][2] = {
		{ -1.0f, -1.0f },
		{  1.0f, -1.0f },
		{  1.0f,  1.0f },
		{ -1.0f,  1.0f },
	};
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof quad / sizeof quad[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);		
	
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

