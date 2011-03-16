/*
 *  XMLSetting.cpp
 *  No.1 Organic Blob
 *
 *  Created by Makira on 11/03/15.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "XMLSetting.h"

void XMLSetting::saveToXMLFile(string tag, string value) {

	xml.clear();
	bool load = xml.loadFile(filePath);
	string curVal = xml.getValue(tag, "init");
	bool exist = xml.tagExists(tag);
	
	cout << "load = " + ofToString(load) << endl;
	cout << "curVal = " + curVal << endl;
	cout << "exist = " + ofToString(exist) << endl;
	
	if (exist && curVal != value) {
		cout << "exist || curVal != value" << endl;
		xml.removeTag(tag);
		xml.addValue(tag, value);
	}else if (!exist) {
		cout << "!exist" << endl;
		xml.addValue(tag, value);
	}
	xml.saveFile(filePath);
	
	cout << " " << endl;

}

void XMLSetting::removeTag(string tag) {
	
	xml.clear();
	bool load = xml.loadFile(filePath);	
	xml.removeTag(tag);
	xml.saveFile(filePath);
	
}

string XMLSetting::getValueByTag(string tag, string defaultVal) {
	
	xml.clear();
	bool load = xml.loadFile(filePath);
	string curVal = xml.getValue(tag, defaultVal);
	return curVal;
	
}

bool XMLSetting::tagExists(string tag) {
	
	xml.clear();
	bool load = xml.loadFile(filePath);	
	return xml.tagExists(tag);
	
}

void XMLSetting::restoreFromXMLFile() {

	

}