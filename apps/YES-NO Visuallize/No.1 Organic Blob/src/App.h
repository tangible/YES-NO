#ifndef _APP
#define _APP

#define FPS 25

#include "ofMain.h"
#include "ofxTween.h"
#include "BlobManager.h"
#include "AdminPanel.h"
#include "HTTPClient.h"

/*
 TODO 
 20110203
 1. bulletの球の数をsmsの返答の比で変えること。
 2. smsが入ってきたら、返答の数によってボールをぶつけること(massをいじればいいかも。そしたらはげしくぶつかるとおもう）
 3. smsの数をどこかに表示すること。返答によって。
*/

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
	void onFileChangeBlobTex(FileDef& fd);	
	void onSMSMsgRecieved(UpdateInfo& upInfo);
	void jitterAndZoomScene();

	BlobManager blobMgr;
	AdminPanel	adminPanel;
	HTTPClient	httpClient;	
	
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

