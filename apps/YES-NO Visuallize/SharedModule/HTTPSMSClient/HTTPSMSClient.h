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
#include "AdminPanel.h"

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
	void setup(AdminPanel* _adminPanel);
	void update(bool _bDebug = false);
	
	AdminPanel* adminPanel;
	
	//--for real server interaction
	void sendRequestToServer(bool bAll = false, bool _bDebug = false, bool _cHull = false);
	void getSMSAnswersFromServer(ofxHttpResponse & response);
	void getAllSMSAnswersFromServer(ofxHttpResponse & response);
	void debugCreateFakeSMSResult(ofxHttpResponse & response);
	void doSequentialNotificationToApps();
	UpdateInfo calcUpdateInfoForRealEnv();
	void createFakeSMS(); // test method
	void createFakeSMS10(); // test method
	void createFakeSMS100(); // test method	
	void startCreateSequentialFakeSMS(); // test method		
	int fakeSMSNum;
	int fakeLastTime;
	int fakeInterval;
	bool bDebug;
	//
	
	
	//--for local emulation
	void sendRequest();
	void emulateSMS(int yna = 3);
	//
	
	
	int				numSMSs;
	static const int tenSec = 10000;
	int				interval;
	int				lastTime;
	vector<smsMsg> smss;
	int				totalSMSs;
	int				totalYes;
	int				totalNo;
	
	
	vector<smsMsg> totalYess;
	vector<smsMsg> totalNos;
	vector<smsMsg> thisTimeYess;
	vector<smsMsg> thisTimeNos;
	vector<string> smsServerRecievedTimes;
	string reqestTime;
	string recieveTime;
	
	ofEvent<UpdateInfo> onSMSRecieved; 
	ofEvent<vector<UpdateInfo> > onAllSMSRecieved;
	
	static const int YES = 0;
	static const int NO = 1;
	
private:
	void getResponse(ofxHttpResponse & response);
	UpdateInfo calcUpdateInfo();	
	string str_replace(const string &source,
					   const string &pattern,
					   const string &placement);	
	bool caseInsCompare(const string& s1, const string& s2);
	
	ofxHttpUtils	realHttpUtils;
	ofxHttpUtils	realAllHttpUtils;
	ofxHttpUtils	makeFakeSMSHttpUtils;
	ofxHttpUtils	testHttpUtils;
	string			responseStr;
	string			action_url;	
	TimedCounter*	requestCounter;
	ofxXmlSettings	xml;
	
	float test;
	
};