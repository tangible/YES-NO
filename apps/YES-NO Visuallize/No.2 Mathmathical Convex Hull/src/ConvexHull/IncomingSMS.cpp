/*
 *  IncomingSMS.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/11.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "IncomingSMS.h"

void IncomingSMS::setup(int _YesOrNo, btSoftBody::tFaceArray faces, int _faceID) {
	
	YesOrNo = _YesOrNo;
	faceID = _faceID;
	btSoftBody::Node* node_0 = faces[faceID].m_n[0];
	btSoftBody::Node* node_1 = faces[faceID].m_n[1];
	btSoftBody::Node* node_2 = faces[faceID].m_n[2];		
	ofxVec3f normal = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[faceID].m_normal);
	
	btnormal = &faces[faceID].m_normal;
	node0 = node_0;
	node1 = node_1;
	node2 = node_2;

	distanceTween.setParameters(circ, ofxTween::easeOut, 1000, 10, 1000, 0);
	bNotify = false;

}

void IncomingSMS::update(btSoftBody::tFaceArray faces) {

	btSoftBody::Node* node_0 = faces[faceID].m_n[0];
	btSoftBody::Node* node_1 = faces[faceID].m_n[1];
	btSoftBody::Node* node_2 = faces[faceID].m_n[2];		
	ofxVec3f normal = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[faceID].m_normal);
	
	btnormal = &faces[faceID].m_normal;
	node0 = node_0;
	node1 = node_1;
	node2 = node_2;

}

void IncomingSMS::draw() {}

void IncomingSMS::debugDraw() {
	
	float factor = distanceTween.update();
	
	ofxVec3f norm = ofxVec3f(btnormal->getX(), btnormal->getY(), btnormal->getZ());
	norm *= factor;
	
	ofxVec3f a = ofxBulletStaticUtil::btVec3ToOfxVec3(node0->m_x);//.rescale(2.0);
	ofxVec3f b = ofxBulletStaticUtil::btVec3ToOfxVec3(node1->m_x);//.rescale(2.0);
	ofxVec3f c = ofxBulletStaticUtil::btVec3ToOfxVec3(node2->m_x);//.rescale(2.0);	
	
	ofxVec3f centroid = ofxVec3f((a.x+b.x+c.x)/3,
								 (a.y+b.y+c.y)/3,
								 (a.z+b.z+c.z)/3);
	ofxVec3f ac = a-centroid;
	ofxVec3f bc = b-centroid;
	ofxVec3f cc = c-centroid;
	float factorC = ofMap(factor, 10, 1000, 0, 10);
	ac *= factorC;
	bc *= factorC;
	cc *= factorC;
	
	ofSetLineWidth(4);
	glDisable(GL_CULL_FACE);	
	glDisable(GL_LIGHTING);
	ofSetColor(0, 0, 0);
	ofxLine(a+norm+ac, b+norm+bc);
	ofxLine(b+norm+bc, c+norm+cc);
	ofxLine(c+norm+cc, a+norm+ac);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	ofSetLineWidth(1);
	
	if (factor < 50 && !bNotify) {
		SmsInfo si;
		si.YesOrNo = YesOrNo;		
		si.faceID = faceID;
		si.a = a+norm;
		si.b = b+norm;
		si.c = c+norm;		
		ofNotifyEvent(onSmsReached, si);
		bNotify = true;
	}
	if (distanceTween.isCompleted()) {
		SmsInfo si;
		si.YesOrNo = YesOrNo;
		si.faceID = faceID;
		si.a = a+norm;
		si.b = b+norm;
		si.c = c+norm;
		ofNotifyEvent(onSmsCompleted, si);
	}
}
