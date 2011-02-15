#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFbo.h"
#include "ofxShader.h"
#include "ofx3DUtils.h"
#include "ofxEasyCam.h"

class testApp : public ofBaseApp{

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
	
//	ofxEasyCam cam;
	
	void drawScene();
	vector<ofxVec4f> objCol;
	vector<ofxVec3f> objPos;
	vector<int> objSize;
	int numObj;
	
	ofxShader pass1;
	ofxShader pass2;

};

#endif
