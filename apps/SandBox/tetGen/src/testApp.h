#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofx3DUtils.h"
#include "ofxEasyCam.h"
#include "ofxColorPicker.h"
#include "tetgen.h"

class tri {
	ofxVec3f v1;
	ofxVec3f v2;
	ofxVec3f v3;
};

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
	
	ofxEasyCam cam;
	ofxColorPicker cp;
	tetgenio* out;
	vector<ofxVec3f> points;

};

#endif
