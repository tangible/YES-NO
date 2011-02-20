#pragma once

#include "ofMain.h"
#include "ofxShader.h"
#include "ofxDepthFBO.h"
#include "ofxColorFBO.h"
#include "AdminPanel.h"
#include "ParticleCloud.h"
#include "ofxEasyCam.h"

class App : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);	
	
	AdminPanel adminPanel;
	ParticleCloud pCloud;
	
	//ofxEasyCam cam;
	ofxCamera cam;
	
	ofxShader defaultShader;
	ofxShader showDepthShader;
	ofxShader ssaoShader;	
	ofxShader dofShader;
	ofxDepthFBO depthFBO;
	ofxColorFBO colorFBO;
	ofxColorFBO ssaoFBO;
	void drawFullScreenQuad(int w, int h);

};

