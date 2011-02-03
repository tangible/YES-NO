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

class smsMsg {
public:
	string answer;
	string time;
};

class UpdateInfo {
public:
	float ratioTotalYes;
	float ratioTotalNo;
	float ratioThisTimeYes;
	float ratioThisTimeNo;
	int numTotalYes;
	int numTotalNo;
	int numYes;
	int numNo;
	string requesttime;
};

class HTTPClient {

public:
	void setup();
	void update();
	void sendRequest();
	void getResponse(ofxHttpResponse & response);
	vector<smsMsg> totalYess;
	vector<smsMsg> totalNos;
	vector<smsMsg> thisTimeYess;
	vector<smsMsg> thisTimeNos;
	string reqestTime;
	string recieveTime;
	
	ofEvent<UpdateInfo> onSMSRecieved; 
	
private:
	string str_replace(const string &source,
					   const string &pattern,
					   const string &placement);	
	UpdateInfo calcUpdateInfo();
	
	ofxHttpUtils	httpUtils;
	string			responseStr;
	string			action_url;	
	TimedCounter*	counter;
	ofxXmlSettings	xml;
};