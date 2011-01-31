/*
 *  HTTPClient.h
 *  AppManager
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#include "ofxHttpUtils.h"
#include "ofxTimer.h"
#include "ofxXmlSettings.h"

struct smsMsg {
	string answer;
	string time;
};

class HTTPClient {

public:
	void setup();
	void update();
	void sendRequest();
	void getResponse(ofxHttpResponse & response);
	
private:
	string str_replace(const string &source,
					   const string &pattern,
					   const string &placement);	
	
	ofxHttpUtils	httpUtils;
	string			responseStr;
	string			action_url;
	vector<smsMsg>	smsMsgs;	
	TimedCounter*	counter;
	ofxXmlSettings	xml;
};