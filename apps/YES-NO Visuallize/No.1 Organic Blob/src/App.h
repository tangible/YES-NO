#ifndef _APP
#define _APP

#define FPS 25

#include "ofMain.h"
#include "ofxTween.h"
#include "BlobManager.h"
#include "AdminPanel.h"
#include "HTTPSMSClient.h"
#include "QuestionImage.h"
#include "StateText.h"

// No.1 Organic Blob
class App : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
	void onFileChangeBG(FileDef& fd);
	void onFileChangeQImg(FileDef& fd);
	void onFileChangeBlobTex(FileDef& fd);	
	void onSMSMsgRecieved(UpdateInfo& _upInfo);
	void onClearQImg(int& i);
	bool bAlreadyRestoreAllAnswer;	
	void onRestoreAllSMSAnswer(int& i);	
	void jitterAndZoomScene();

	BlobManager		blobMgr;
	AdminPanel		adminPanel;
	HTTPSMSClient	httpClient;	
	QuestionImage	qImage;
	StateText		sText;
	UpdateInfo		upInfo;
	
	ofxCamera		camera;
	ofxTween		cameraXTween;
	ofxTween		eyeXTween;
	ofxTween		cameraYTween;
	ofxTween		eyeYTween;
	ofxTween		cameraZTween;	
	ofxTween		bgJitterTween;
	
	
	// tweens
	vector<ofxEasing>   easing;
	ofxEasingBack		easingback;
	ofxEasingBounce 	easingbounce;
	ofxEasingCirc		easingcirc;
	ofxEasingCubic		easingcubic;
	ofxEasingElastic	easingelastic;
	ofxEasingExpo		easingexpo;
	ofxEasingLinear 	easinglinear;
	ofxEasingQuad		easingquad;
	ofxEasingQuart		easingquart;
	ofxEasingQuint		easingquint;
	ofxEasingSine		easingsine;	
};

#endif

