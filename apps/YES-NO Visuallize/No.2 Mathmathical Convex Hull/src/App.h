#pragma once

#include "ofMain.h"
#include "ofxEasyCam.h"
#include "ConvexHull.h"
#include "AdminPanel.h"
#include "HTTPSMSClient.h"
#include "StateText.h"
#include "QuestionImage.h"
#include "ofxTween.h"

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
	
	void changeImgBG(string path);	
	void onFileChangeBG(FileDef& fd);	
	void onClearBG(int& i);
	void onFileChangeQImg(FileDef& fd);	
	void onClearQImg(int& i);	
	void onSMSMsgRecieved(UpdateInfo& _upInfo);	
	bool bAlreadyRestoreAllAnswer;	
	void onRestoreAllSMSAnswer(int& i);
	void resotoreCamOrbit(int & z);
	void camOrbit(int & z);

//	void onNotifyUpdateStextColorYesEvent(int & z) { sText.updateColorYes(convexHull.getYesSpikeColor()); }
//	void onNotifyUpdateStextColorNoEvent(int & z) { sText.updateColorNo(convexHull.getNoSpikeColor()); }
	void onNotifyUpdateStextColorYesEvent(int & z) { sText.updateColorYes(convexHull.getYesSpikeColorFloat()); }
	void onNotifyUpdateStextColorNoEvent(int & z) { sText.updateColorNo(convexHull.getNoSpikeColorFloat()); }	
	void onNotifyStartStextFadingYesEvent(int & z) { sText.startFadeToDefaultColorYes(); };
	void onNotifyStartStextFadingNoEvent(int & z) { sText.startFadeToDefaultColorNo(); };	
	
	AdminPanel		adminPanel;
	ConvexHull		convexHull;
	HTTPSMSClient	httpClient;
	StateText		sText;
	QuestionImage	qImage;
	UpdateInfo		upInfo;		
	
	bool bgenY;
	
	ofImage bg;
	ofVideoPlayer*		bgPlayer;	
	bool				isVidBG;	
	vector<UpdateInfo> smsQue;
	
	//ofxEasyCam		cam;
	ofxCamera		cam;
	ofxTween		camOrbitTween;
//	ofxEasingQuad	camOrbitEasing;
	ofxEasingSine	camOrbitEasing;	
	float			camOrbitAmt;
	int				prevOrbit;

};

