/*
 *  HTTPSMSClient.h
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
	int YesOrNo;	
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
	int numDiffYes;
	int numDiffNo;
	smsMsg sms;
	string requesttime;
};

class HTTPSMSClient {

public:
	void setup();
	void update();
	void sendRequest();
	void getResponse(ofxHttpResponse & response);
	void emulateSMS();
	vector<smsMsg> totalYess;
	vector<smsMsg> totalNos;
	vector<smsMsg> thisTimeYess;
	vector<smsMsg> thisTimeNos;
	string reqestTime;
	string recieveTime;
	
	ofEvent<UpdateInfo> onSMSRecieved; 
	
	static const int YES = 0;
	static const int NO = 1;
	
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
	
	float test;
	
};