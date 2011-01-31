#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxHttpUtils.h"
#include "ofxTimer.h"
#include "ofxXmlSettings.h"

struct smsMsg {
	string answer;
	string time;
};

class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button); 
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	void sendRequest();
	void getResponse(ofxHttpResponse & response);
	string str_replace(const string &source,
					   const string &pattern,
					   const string &placement);	

	ofxHttpUtils httpUtils;
	string responseStr;
	string action_url;
	vector<smsMsg> smsMsgs;
	
	TimedCounter* counter;

	ofxXmlSettings xml;
	
};

#endif
