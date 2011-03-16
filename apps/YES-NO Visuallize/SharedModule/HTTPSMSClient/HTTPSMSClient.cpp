/*
 *  HTTPSMSClient.cpp
 *  AppManager
 *
 *  Created by Makira on 11/01/31.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "HTTPSMSClient.h"

void HTTPSMSClient::setup(AdminPanel* _adminPanel){
	
	adminPanel = _adminPanel;

	ofAddListener(testHttpUtils.newResponseEvent, this, &HTTPSMSClient::getResponse);
	testHttpUtils.start();
	
	ofAddListener(realHttpUtils.newResponseEvent, this, &HTTPSMSClient::getSMSAnswersFromServer);
	realHttpUtils.start();
	
	ofAddListener(makeFakeSMSHttpUtils.newResponseEvent, this, &HTTPSMSClient::debugCreateFakeSMSResult);
	makeFakeSMSHttpUtils.start();
	
	requestCounter = new TimedCounter(10);
	requestCounter->startCount();	

	numSMSs = 0;
	interval = 0;
	lastTime = 0;
	totalSMSs = 0;
	totalYes = 0;	
	totalNo = 0;	
	fakeSMSNum = 0;
	fakeInterval = 0;
	
};

void HTTPSMSClient::update(bool _bDebug){

	if(requestCounter->isCounting()){
		requestCounter->update();
		if (requestCounter->isCountComplete()) {
			sendRequestToServer(false, _bDebug);
			requestCounter->startCount();
		}
	}
	
	doSequentialNotificationToApps();
	
	if (bDebug) startCreateSequentialFakeSMS();
};


void HTTPSMSClient::sendRequestToServer(bool bAll, bool _bDebug) {
	
	bDebug = _bDebug;
	
	ofxHttpForm form;
	form.action = "http://hub.tangibleinteraction.com/api/entries?api_key=8ff2812eacf66449e6008425e2ae5a3a0d18cb22d089e4d5c6933c2c298aa678";
	string installation = "&installation_id=";
	string installation_id = "855181531";
	form.action += installation;
	form.action += installation_id;
	
	
	// add time param to url
	if (!bAll) {
		time_t current;
		time(&current);
		char gmttime[256];
		strftime(gmttime, 255, "%Y¥-%m-%d¥ %H¥:%M¥:%S", gmtime(&current));
		string currentStr = gmttime;
		currentStr = str_replace(currentStr, "¥", "");		
		cout << currentStr << endl;
		time_t tenSecPast = current - 9;
		strftime(gmttime, 255, "%Y¥-%m-%d¥ %H¥:%M¥:%S", gmtime(&tenSecPast));
		string pastStr = gmttime;
		pastStr = str_replace(pastStr, "¥", "");			
		cout << pastStr << endl;
		
		string start_date = "&start_date=" + pastStr;
		string end_date = "&end_date=" + currentStr;
		form.action += start_date;
		form.action += end_date;
	}
	
	if (bDebug) {
		fakeSMSNum = ofRandom(1, 10);
		fakeLastTime = ofGetElapsedTimeMillis();
		fakeInterval = tenSec/fakeSMSNum;
	}
	
	cout << form.action << endl;
	realHttpUtils.addForm(form);
	
}

void HTTPSMSClient::getSMSAnswersFromServer(ofxHttpResponse & response) {

	responseStr.clear();
	xml.clear();
	xml.loadFromBuffer(response.responseBody);	
	
	// debug block
	{
		string test;
		xml.copyXmlToString(test);
		cout << test << endl;
	}	
	
	xml.pushTag("entries");
	int numTag = xml.getNumTags("entry");
	
	int numYes = 0;
	int numNo = 0;
	
	for (int i = 0; i < numTag; i++) {
		xml.pushTag("entry", i);
		string from = xml.getValue("from", "error");
		string question_id = xml.getValue("question_id", "error");
		string ans = xml.getValue("answer", "error");
		string time = xml.getValue("time", "error");			
		
		// if we get the entry which bind to this App
		cout << "question_id = "+question_id+" adminPanel->phone_questionID = "+adminPanel->phone_questionID << endl;
		if ((question_id != "error") && question_id == adminPanel->phone_questionID) {		
			smsMsg sms;
			sms.answer = ans;
			sms.time = time;
			
			if (caseInsCompare("Yes", sms.answer)) {
				sms.YesOrNo = 0;
				numYes++;
			}else {
				sms.YesOrNo = 1;
				numNo++;
			}
			
			smss.push_back(sms);
		}
		
		xml.popTag();		
	}
	xml.popTag();	
	
//	cout << "num yes = "+ofToString(numYes) << endl;
//	cout << "num no = "+ofToString(numNo) << endl;
	
	numSMSs = numTag;
	if (numSMSs == 0) {
		interval = -100;
	}else {
		interval = tenSec/numSMSs;
	}
	lastTime = ofGetElapsedTimeMillis();
	cout << "recieve answer @ " + ofToString(lastTime) << endl;
	
}

void HTTPSMSClient::doSequentialNotificationToApps() {
	
	if (smss.size() > 0) {
		int curTime = ofGetElapsedTimeMillis();
		//cout << "curTime : " + ofToString(curTime) << endl;
		if (curTime > lastTime+interval) {
			
			UpdateInfo upInfo = calcUpdateInfoForRealEnv();
			ofNotifyEvent(onSMSRecieved, upInfo);			
			
			//numSMSs--;
			lastTime += interval;			
		}
	}
	
}

void HTTPSMSClient::startCreateSequentialFakeSMS() {
	
	if (fakeSMSNum > 0) {
		int curTime = ofGetElapsedTimeMillis();
		if (curTime > fakeLastTime+fakeInterval) {
			createFakeSMS();
			fakeLastTime += fakeInterval;
			fakeSMSNum--;
		}
	}
	
}

UpdateInfo HTTPSMSClient::calcUpdateInfoForRealEnv() {
	
	
	cout << "sms size = "+ofToString((int)smss.size()) << endl;	
	
	smsMsg sms = smss[smss.size()-1];
	smss.pop_back();
	UpdateInfo upInfo;
	
	int thisYesOrNo = sms.YesOrNo;
	totalSMSs++;
	if (0 == thisYesOrNo) {
		cout << "yes!!" << endl;
		totalYes++;
	}else {
		cout << "no!!" << endl;
		totalNo++;
	}
//	cout << "totalYes = "+ofToString(totalYes) << endl;
//	cout << "totalNo = "+ofToString(totalNo) << endl;	
	
	float rtotalYes = totalYes;
	float rtotalNo = totalNo;
	float rthisTimeYes = (thisYesOrNo==0)?1:0;
	float rthisTimeNo = (thisYesOrNo==1)?1:0;
	
	float rtotalSMS = totalSMSs;
	float rtotalSMSThisTime = 1;
	float raTotalYes = (totalYes<=0)?0.0:rtotalYes/rtotalSMS;
	float raTotalNo = (totalNo<=0)?0.0:rtotalNo/rtotalSMS;
	float raThisTimeYes = (rthisTimeYes<=0)?0.0:rthisTimeYes/rtotalSMSThisTime;
	float raThisTimeNo = (rthisTimeNo<=0)?0.0:rthisTimeNo/rtotalSMSThisTime;
	
	upInfo.sms = sms;
	
	upInfo.ratioTotalYes = raTotalYes;
	upInfo.ratioTotalNo = raTotalNo;
	upInfo.ratioThisTimeYes = raThisTimeYes;
	upInfo.ratioThisTimeNo = raThisTimeNo;
	upInfo.numTotalYes = rtotalYes;
	upInfo.numTotalNo = rtotalNo;
	upInfo.numYes = rthisTimeYes;
	upInfo.numNo = rthisTimeNo;
	upInfo.numDiffYes = rtotalYes-rtotalNo;
	upInfo.numDiffNo = rtotalNo-rtotalYes;
	upInfo.requesttime = reqestTime;
	
	//	cout << "thisTimeYes recieved = " + ofToString(upInfo.numYes) << endl;	
	//	cout << "totalYes recieved = " + ofToString(upInfo.numTotalYes) << endl;
	
	return upInfo;	
	
}

void HTTPSMSClient::createFakeSMS() {
	
	ofxHttpForm form;
	string url = "http://hub.tangibleinteraction.com/api/anveo";
	
	string from = "?from=";
	string from_value = adminPanel->kioskPhoneNum_asFrom;
	string phonenumber = "&phonenumber=";
	string phonenumber_value = adminPanel->phone_questionID;
	
	string message = "&message=";
	string message_value = "Yes";
	if (0.0 < ofRandomf()) {
		message_value = "No";
	}
		
	string otherParams = "&api_key=8ff2812eacf66449e6008425e2ae5a3a0d18cb22d089e4d5c6933c2c298aa678&installation_id=855181531&test=true";

	form.action = 
	url+
	from+from_value+
	phonenumber+phonenumber_value+
	message+message_value+
	otherParams;
	
	cout << "action url = " << endl;
	cout << form.action << endl;
	
	makeFakeSMSHttpUtils.addForm(form);	
	
}

void HTTPSMSClient::debugCreateFakeSMSResult(ofxHttpResponse & response) {
	
	cout << "create SMS result :: "+response.responseBody << endl;
	
}







void HTTPSMSClient::sendRequest(){

	ofxHttpForm form;
	form.action = "http://localhost:8888/getSMS.of";
	form.method = OFX_HTTP_POST;
	
	time_t t;
	time(&t);
	char gmttime[256];
	strftime(gmttime, 255, "%Y¥-%m-%d¥%H¥:%I¥:%S", gmtime(&t));
	string timestr = gmttime;
	timestr = str_replace(timestr, "¥", "");			
	reqestTime = timestr;
	
	form.addFormField("time", timestr);
	testHttpUtils.addForm(form);	

};

void HTTPSMSClient::emulateSMS(int yna) { // 0 = yes, 1 = no, other = random
	
	ofxHttpResponse response;

	string res = "";
	string start = "<TangibleHUBYesNoResponce><SMSs>";
	
	time_t t;
	time(&t);
	char gmttime[256];
	strftime(gmttime, 255, "%Y¥-%m-%d¥%H¥:%I¥:%S", gmtime(&t));
	string timestr = gmttime;
	timestr = str_replace(timestr, "¥", "");		
	
	string yes = "<SMS><Answer>YES</Answer><Time>" + timestr + "</Time></SMS>";
	string no = "<SMS><Answer>NO</Answer><Time>" + timestr + "</Time></SMS>";
	string contents = "";
	int thisreq = 1;//ofRandom(1, 20);
	for (int i = 0; i < thisreq; i++) {
		
		
		if (yna == 0) {
			test = 0;
			contents += yes;
		}else if (yna == 1) {
			test = 1;
			contents += no;
		}else {
			float rdm = ofRandomf();
			if (rdm < 0) {
				test = 0;
				contents += yes;
			}else {
				test = 1;
				contents += no;
			}
		}

	}
	
	string end = "</SMSs></TangibleHUBYesNoResponce>";
	res = start+contents+end;
	
	response.responseBody = res;
	getResponse(response);
	
}
	
UpdateInfo HTTPSMSClient::calcUpdateInfo() {
	
	UpdateInfo upInfo;
	
	float totalYes = totalYess.size();
	float totalNo = totalNos.size();
	float thisTimeYes = thisTimeYess.size();
	float thisTimeNo = thisTimeNos.size();
	
	float totalSMS = totalYes+totalNo;
	float totalSMSThisTime = thisTimeYes+thisTimeNo;
	float raTotalYes = (totalYes<=0)?0.0:totalYes/totalSMS;
	float raTotalNo = (totalNo<=0)?0.0:totalNo/totalSMS;
	float raThisTimeYes = (thisTimeYes<=0)?0.0:thisTimeYes/totalSMSThisTime;
	float raThisTimeNo = (thisTimeNo<=0)?0.0:thisTimeNo/totalSMSThisTime;
	
	smsMsg sms;
	if (test == 0) {
		sms.YesOrNo = HTTPSMSClient::YES;
		sms.answer = "YES";
	}else {
		sms.YesOrNo = HTTPSMSClient::NO;
		sms.answer = "NO";
	}
	upInfo.sms = sms;
	
	upInfo.ratioTotalYes = raTotalYes;
	upInfo.ratioTotalNo = raTotalNo;
	upInfo.ratioThisTimeYes = raThisTimeYes;
	upInfo.ratioThisTimeNo = raThisTimeNo;
	upInfo.numTotalYes = totalYes;
	upInfo.numTotalNo = totalNo;
	upInfo.numYes = thisTimeYes;
	upInfo.numNo = thisTimeNo;
	upInfo.numDiffYes = totalYes-totalNo;
	upInfo.numDiffNo = totalNo-totalYes;
	upInfo.requesttime = reqestTime;
	
	//	cout << "thisTimeYes recieved = " + ofToString(upInfo.numYes) << endl;	
	//	cout << "totalYes recieved = " + ofToString(upInfo.numTotalYes) << endl;
	
	return upInfo;
	
}

void HTTPSMSClient::getResponse(ofxHttpResponse & response){
	
	responseStr.clear();
	
	time_t t;
	time(&t);
	char gmttime[256];
	strftime(gmttime, 255, "%Y¥-%m-%d¥%H¥:%I¥:%S", gmtime(&t));
	string timestr = gmttime;
	timestr = str_replace(timestr, "¥", "");		
	recieveTime = timestr;
	
	thisTimeYess.clear();
	thisTimeNos.clear();
	
	xml.clear();
	xml.loadFromBuffer(response.responseBody);
	
	// debug block
	//	{
	//		string test;
	//		xml.copyXmlToString(test);
	//		cout << test << endl;
	//		
	//		xml.pushTag("TangibleHUBYesNoResponce");
	//		xml.pushTag("SMSs");
	//		int tansmss = xml.getNumTags("SMS");
	//		cout << "TangibleHUBYesNoResponce:SMSs:SMS = " + ofToString(tansmss) << endl;
	//		xml.popTag();
	//		xml.popTag();
	//	}
	
	xml.pushTag("TangibleHUBYesNoResponce");
	xml.pushTag("SMSs");
	int numTag = xml.getNumTags("SMS");
	for (int i = 0; i < numTag; i++) {
		xml.pushTag("SMS", i);
		string ans = xml.getValue("Answer", "error");
		string time = xml.getValue("Time", "error");
		responseStr += " "+ans+" "+time+" ";
		
		smsMsg sms;
		sms.answer = ans;
		sms.time = time;
		
		if ("YES" == sms.answer) {
			totalYess.push_back(sms);
			thisTimeYess.push_back(sms);
		}else {
			totalNos.push_back(sms);
			thisTimeNos.push_back(sms);
		}
		
		xml.popTag();
	}
	xml.popTag();
	xml.popTag();	
	
	UpdateInfo upInfo = calcUpdateInfo();
	ofNotifyEvent(onSMSRecieved, upInfo);
	
};

string HTTPSMSClient::str_replace(const string &source,
							   const string &pattern,
							   const string &placement){

    string result;
    string::size_type pos_before = 0;
    string::size_type pos = 0;
    string::size_type len = pattern.size();
    while ((pos = source.find(pattern, pos)) != string::npos) {
        result.append(source, pos_before, pos - pos_before);
        result.append(placement);
        pos += len;
        pos_before = pos;
    }
    result.append(source, pos_before, source.size() - pos_before);
    return result;

}

inline bool caseInsCharCompareN(char a, char b) {
	return(toupper(a) == toupper(b));
}
bool HTTPSMSClient::caseInsCompare(const string& s1, const string& s2) {
	return((s1.size( ) == s2.size( )) &&
		   equal(s1.begin( ), s1.end( ), s2.begin( ), caseInsCharCompareN));
}
