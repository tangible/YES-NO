#pragma once

#include "ofMain.h"
#include "ofxEasyCam.h"
#include "ConvexHull.h"
#include "AdminPanel.h"
#include "HTTPSMSClient.h"
#include "StateText.h"
#include "QuestionImage.h"

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
	
	void onFileChangeBG(FileDef& fd);	
	void onClearBG(int& i);
	void onFileChangeQImg(FileDef& fd);	
	void onClearQImg(int& i);	
	void onSMSMsgRecieved(UpdateInfo& _upInfo);	

	AdminPanel		adminPanel;
	ConvexHull		convexHull;
	HTTPSMSClient	httpClient;
	StateText		sText;
	QuestionImage	qImage;
	
	ofImage bg;
	UpdateInfo upInfo;	
	
//	ofxEasyCam		cam;
	ofxCamera		cam;

};

