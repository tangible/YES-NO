#pragma once

#include "ofMain.h"
#include "ofxShader.h"
#include "ofxFBO.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void texRect(int w, int h);
	void runShader(ofxFBO& fbo, ofxShader& shader);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void setTexture(char* name, ofxShader& shader, ofImage& img, int location);

	ofxShader blendShader;
	ofxFBO fbo;
	ofImage left, right;

	int w, h;

	bool isMousePressed;
};
