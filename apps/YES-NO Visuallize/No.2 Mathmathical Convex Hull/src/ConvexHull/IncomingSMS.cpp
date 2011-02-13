/*
 *  IncomingSMS.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/11.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "IncomingSMS.h"

void IncomingSMS::setup(btSoftBody::tFaceArray faces) {
	
	int rdmIdx = ofRandom(0, faces.size());
	faceID = rdmIdx;
	btSoftBody::Node* node_0 = faces[rdmIdx].m_n[0];
	btSoftBody::Node* node_1 = faces[rdmIdx].m_n[1];
	btSoftBody::Node* node_2 = faces[rdmIdx].m_n[2];		
	ofxVec3f normal = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[rdmIdx].m_normal);
	
	btnormal = &faces[rdmIdx].m_normal;
	node0 = node_0;
	node1 = node_1;
	node2 = node_2;

	distanceTween.setParameters(circ, ofxTween::easeOut, 1000, 1, 1000, 0);
	bNotify = false;

}

void IncomingSMS::update(ofxVec3f destination) {}

void IncomingSMS::draw() {}

void IncomingSMS::debugDraw() {

	
	float factor = distanceTween.update();
	
	ofxVec3f norm = ofxVec3f(btnormal->getX(), btnormal->getY(), btnormal->getZ());
	norm *= factor;
	
	ofxVec3f a = ofxBulletStaticUtil::btVec3ToOfxVec3(node0->m_x);
	ofxVec3f b = ofxBulletStaticUtil::btVec3ToOfxVec3(node1->m_x);
	ofxVec3f c = ofxBulletStaticUtil::btVec3ToOfxVec3(node2->m_x);	
	
	glDisable(GL_CULL_FACE);
	ofSetColor(0, 0, 0);
	ofPushMatrix();
	ofTranslate(norm.x, norm.y, norm.z);	
	ofxTriangleShape(a, b, c);
	ofPopMatrix();
	glEnable(GL_CULL_FACE);
	
	if (factor < 50 && !bNotify) {
		ofNotifyEvent(onSmsReached, faceID);
		bNotify = true;
	}

}
