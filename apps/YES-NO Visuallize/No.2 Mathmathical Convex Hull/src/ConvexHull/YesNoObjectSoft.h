/*
 *  YesNoObjectSoft.h
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/12.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBullet.h"
#include "ofxColorPicker.h"

class YesNoObjectSoft {

public:
	
	int resolusion;
	static const int minRes = 16;
	static const int maxRes = 512;
	ofxVec3f radius;
	
	void setup(ofxBullet* _bullet, ofxVec3f _forcePoint, int res, ofxVec3f scale);
	void update();
	void draw();
	void debugDraw();
	vector<float> caclColAngleAndScaleForObj(btSoftBody::tFaceArray& faces);
	vector<float>  changeColBySMSRecievedFace(btSoftBody::tFaceArray& faces, int faceID);
	vector<int> sortFaceByDistance(btSoftBody::tFaceArray& faces, int faceID);
	ofxVec3f getFaceCentroid(btSoftBody::tFaceArray& faces, int faceID);
	
	vector<float> colorPointer;
	
	void debugKeyPressed(int key);
	
	void addSMS(int faceID);
	void blowUp();
	void shrink();
	void pinchAndSpreadNode();
	void pinchFace(int face);
	void pinchNode();
	
	ofxBullet*	bullet;
	MySoftBody*	yesORno;
	ofxColorPicker col;
	
	int			YesOrNo;
	float		maxValu;
	ofxVec3f	forcePoint;
	
};