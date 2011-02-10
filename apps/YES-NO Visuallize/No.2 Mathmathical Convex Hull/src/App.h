#pragma once

#include "ofMain.h"
#include "HTTPSMSClient.h"
#include "ConvexHull.h"
#include "AdminPanel.h"

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

	AdminPanel		adminPanel;
	ConvexHull		convexHull;
	ofxCamera		cam;
	
};

