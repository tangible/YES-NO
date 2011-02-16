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
	ofBackground(255, 255, 255);
	glEnable(GL_DEPTH_TEST);
	
	ofxSetSphereResolution(100);
	
	cam.setup(this, 700);
	
	defaultShader.setup("default");
	showDepthShader.setup("showdepth");
	ssaoShader.setup("ssao");
	
	depthFBO.setup(ofGetWidth(), ofGetHeight());
	colorFBO.setup(ofGetWidth(), ofGetHeight());
	
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
	
    materialAmbient  = new float[4];
    materialDiffuse  = new float[4];
    materialSpecular = new float[4];
    for (int i=0; i<4; i++){
        materialAmbient[i]  = 1.0f;
        materialDiffuse[i]  = 1.0f;
        materialSpecular[i] = 1.0f;
    }	
	
	//.setNewColumn(true);
	gui.addTitle("Shader Setting");
	gui.addSlider("camerarangex", camerarangex, 0, 10000);
	gui.addSlider("camerarangey", camerarangey, 0, 10000);
	gui.addSlider("aoCap", aoCap, 0.0, 2.0);
	gui.addSlider("aoMultiplier", aoMultiplier, 0.0, 20000.0);
	gui.addSlider("depthTolerance", depthTolerance, 0.000, 0.002);
	gui.addSlider("aorange", aorange, 0.0, 2.0);
	gui.loadFromXML();
	gui.show();		
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
	
	ofSetupScreen();
	
	int colorTexSlot = 4;
	colorFBO.beforeDraw(colorTexSlot);	
	int depthTexSlot = 5;
	depthFBO.beforeDraw(depthTexSlot);
	ssaoShader.begin();
	ssaoShader.setUniform1i("texture0", depthTexSlot);
	ssaoShader.setUniform1i("texture1", colorTexSlot);
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
	depthFBO.afterDraw();
	colorFBO.afterDraw();
	
	
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
	
	gui.draw();
}

//--------------------------------------------------------------
void testApp::drawScene() {
	
	//setupGLStuff();
	
	for (int i = 0; i < numObj; i++) {
		ofxVec3f pos = objPos[i];
		ofxVec4f col = objCol[i];
		int size = objSize[i];
		ofSetColor(col.x, col.y, col.z);
		ofxSphere(pos.x, pos.y, pos.z, size);
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

void testApp::setupGLStuff(){
	
    glEnable(GL_POLYGON_SMOOTH);
//    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

//    glDisable(GL_BLEND);
    glPolygonMode(GL_BACK, GL_FILL );
//	  glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);
    glShadeModel(GL_SMOOTH);
	
    glColor3f(0.5, 0.5, 0.5);
    GLfloat on[]  = {1.0};
    GLfloat off[] = {0.0};
    glLightModelfv( GL_LIGHT_MODEL_TWO_SIDE,on);
	
	
    GLfloat shininess[] = {0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,  materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,  materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
	
	
    GLfloat lightPosition[] = { 0.0f, 0.0f, -100.0, 0.0f };
    GLfloat lightDiffuse[]  = { 1.00, 0.99, 0.98, 1.0};
    GLfloat lightSpecular[] = { 0.10, 0.10, 0.10, 1.0};
    GLfloat lightAmbient[]  = { 0.45, 0.43, 0.44, 1.0};
    glLightfv(GL_LIGHT0,GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0,GL_AMBIENT,  lightAmbient);
	
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

