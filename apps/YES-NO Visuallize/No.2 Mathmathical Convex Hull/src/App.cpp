#include "App.h"

const int fps = 40;

//--------------------------------------------------------------
void App::setup(){

	ofSetFrameRate(fps);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetDataPathRoot("../Resources/");
	ofBackground(255, 255, 255);
	
//	cam.position(ofGetWidth()/2, ofGetHeight()/2+200, 1200);
	cam.setup(this, 1200);
	
	adminPanel.setup();
	stateText.setup();	
	convexHull.setup(fps, &adminPanel, &stateText, &cam);
	
	shader.setup("phong");
	
}

//--------------------------------------------------------------
void App::update(){
	
	adminPanel.update();
	convexHull.update();
	
}

//--------------------------------------------------------------
void App::draw(){
	
//	cam.place();
	cam.draw();
	
	//shader.begin();
	shader.setUniform3f("AmbientColour", adminPanel.MATERIALAMBIENT[0], adminPanel.MATERIALAMBIENT[1], adminPanel.MATERIALAMBIENT[2]);
	shader.setUniform3f("DiffuseColour", adminPanel.MATERIALDIFFUSE[0], adminPanel.MATERIALDIFFUSE[1], adminPanel.MATERIALDIFFUSE[2]);
	shader.setUniform3f("SpecularColour", adminPanel.MATERIALSPECULAR[0], adminPanel.MATERIALSPECULAR[1], adminPanel.MATERIALSPECULAR[2]);
	shader.setUniform1f("AmbientIntensity", adminPanel.AMBIENTINTENSITY);
	shader.setUniform1f("DiffuseIntensity", adminPanel.DEFFUSEINTENSITY);
	shader.setUniform1f("SpecularIntensity", adminPanel.SPECULARINTENSITY);
	shader.setUniform1f("Roughness", adminPanel.ROUGHNESS);
	shader.setUniform1f("Sharpness", adminPanel.SHARPNESS);	
	shader.setUniform3f("LightPos", adminPanel.LIGHTX, adminPanel.LIGHTY, adminPanel.LIGHTZ);
	convexHull.draw(mouseX, mouseY);
	//shader.end();
	
	
	ofSetupScreen();
	adminPanel.draw();	
	
}

//--------------------------------------------------------------
void App::keyPressed(int key){

	convexHull.keyPressed(key);
	if (key == ' ') adminPanel.toggle();
	
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

