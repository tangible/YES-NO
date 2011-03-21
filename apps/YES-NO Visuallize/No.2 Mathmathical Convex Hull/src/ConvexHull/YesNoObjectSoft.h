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
#include "ofxTween.h"
#include "ofxTimer.h"
#include "HTTPSMSClient.h"
#include "ofxVCGLib.h"

static float verts[24] = {
	
	(-106.7201920), (154.0109253), (-65.5737534),
	(122.8457947), (178.0053253), (27.6124306),
	(5.0930414), (16.9040928), (-152.4526672),
	(-193.7199860), (-31.0964642), (-114.1005859),
	(-262.1704407), (67.5808640), (104.2402115),
	(196.7726746), (-28.3008862), (158.6202545),
	(-117.9212341), (-143.9787903), (61.4517021),
	(124.9667053), (-139.3863831), (-20.1715240)
	
};

class addedSMS {
	
public:
	int faceID;
	float length;
	btVector3* btnormal;
	btSoftBody::Node* node0;
	btSoftBody::Node* node1;
	btSoftBody::Node* node2;	
	float angle;
	ofxTween tw;
	ofxEasingElastic ea;
	ofEvent<int> endSpikeGlow;
	
	addedSMS() {
		ofAddListener(tw.end_E, this, &addedSMS::notifyEndSpikeGlow);
		btnormal = NULL;
		node0 = NULL;
		node1 = NULL;
		node2 = NULL;
	}
	~addedSMS() {
//		if (btnormal != NULL) delete btnormal;
//		if (node0 != NULL) delete node0;
//		if (node1 != NULL) delete node1;
//		if (node2 != NULL) delete node2;
	}
	
	inline void notifyEndSpikeGlow(int & z) {
		int i = 0;
		ofNotifyEvent(endSpikeGlow, i);
	}
	
	ofxVec3f getA() {
		return ofxBulletStaticUtil::btVec3ToOfxVec3(node0->m_x);
	}
	ofxVec3f getB() {
		return ofxBulletStaticUtil::btVec3ToOfxVec3(node1->m_x);
	}
	ofxVec3f getC() {
		return ofxBulletStaticUtil::btVec3ToOfxVec3(node2->m_x);
	}
	ofxVec3f getNorm() {
		return ofxVec3f(btnormal->getX(), btnormal->getY(), btnormal->getZ());
	}
};


class YesNoObjectSoft  : public ofxThread{

public:
	
	void setup(int _yesOrNo, ofxBullet* _bullet, ofxVec3f _forcePoint, ofxVec3f scale, int _sizeLevel);
	void update();
	void draw();
	void debugDraw();
	vector<float> changeColBySMSRecievedFace(int z);	
	void updateColorPointer();
	void startFaceingToCam(ofxCamera* cam, ofxVec3f offset);
	void updateRotateion();
	void updateTranslation();
	
	void addSMS(int faceID, int _numSMS, float _ratioSMS);
	void addSMSCompleted(int & z);
	
	void genShapeAtOnce(int numEvolve);
	int genShapeProgramatically();
	
	void clear();
	
	ofSoundPlayer			sound;	
	ofColor					currentSpikeColor;
	float					currentSpikeColorFloat;
	
	int incomingSMSFaceID;	
	UpdateInfo updateInfo;
	int numSMS;
	float ratioSMS;
	
	ofEvent<int> onFinishAllUpdating;
	ofEvent<int> notifyScaleYesEvent;	
	ofEvent<int> notifyScaleNoEvent;	
	void notifyFinishAllUpdating(int & z);
	ofEvent<int> notifyStartCamOrbit;
	ofEvent<int> notifyUpdateStextColorYesEvent;
	ofEvent<int> notifyUpdateStextColorNoEvent;
	ofEvent<int> notifyStartStextFadingYesEvent;
	ofEvent<int> notifyStartStextFadingNoEvent;	
	
	ofxBullet*	bullet;
	MySoftBody*	yesORno;
	ofxVCGLib vcgMesh;
	ofxColorPicker col;	
	float defaultColAng;
	float previousColAng;
	bool suddenMotion;
	
	static const int YES = 0;
	static const int NO = 0;
	static const int minRes = 5;
	static const int maxRes = 5120;
	static const int YNSOFTMINSIZELEV = 0;
	static const int YNSOFTMAXSIZELEV = 20;	
	static const int maxSMSNum = 5000;
	int resolusion;	
	ofxVec3f radius;
	int sizeLevel;
	float height;
	float edgeLen;
	
	vector<float> destColorPointer;
	vector<float> currColorPointer;
	ofxTween	  colorPointerTween;
	ofxEasingCirc easing;
	
	vector<addedSMS*> addedSMSs;
	vector<ofxVec3f> addedSMSFaces;
	ofxVec3f smsPosition;	
	vector<int> faceIndices;
	vector<float> floatVertices;
	int numFace;
	void onEndSpikeGlow(int & z);
	void fillMeshInput();
	vector<ofxVec3f> smsBaseFace; //debug
	
	int			YesOrNo;
	ofxVec3f	forcePoint;
	ofxVec3f	objCentroid;
	ofxVec3f    preservedObjCentroid;
	
	ofxTween quatTween;
	ofxEasingCirc quatEasing;
	ofxQuaternion from;
	ofxQuaternion to;
	ofxVec3f prevFacingAxis;
	float prevFaceAngle;	
	
	ofxTween translationTween;
	ofxEasingCirc translationEasing;
	
	static const float maxNumScale = 2.0;
	static const float maxRatioScale = 6.0;

//	float curScale;
//	ofxTween scaleTween;
//	ofxEasingBounce scaleEasing;
//	void setCurrentScale(int & z);
//	float getScale();
	
};