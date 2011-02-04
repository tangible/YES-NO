#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxHTTPServer.h" 



class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	void getRequest(ofxHTTPServerResponse & response);
	void postRequest(ofxHTTPServerResponse & response);
	void processRequest(ofxHTTPServerResponse & response);
	string str_replace(const string &source,
					   const string &pattern,
					   const string &placement);	

	ofxHTTPServer * server;
	string urlParam;	
	
	ofTrueTypeFont font;
	int r, g, b;
	
	int ratio;
};

#endif
