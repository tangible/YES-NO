/*
 *  XMLSetting.h
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/03/15.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

static const string filePath = "AppSpecificSetting.xml";

class XMLSetting {

public:
	ofxXmlSettings xml;
	
	void saveToXMLFile(string tag, string value);
	void removeTag(string tag);
	string getValueByTag(string tag, string defaultVal);
	bool tagExists(string tag);
	void restoreFromXMLFile();	

};