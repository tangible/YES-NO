/*
 *  YesNoObjectSoft.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/12.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "YesNoObjectSoft.h"

void YesNoObjectSoft::setup(int _yesOrNo, ofxBullet* _bullet, ofxVec3f _forcePoint, ofxVec3f scale, int _sizeLevel) {

	YesOrNo = _yesOrNo;
	bullet = _bullet;
	forcePoint = _forcePoint;
	sizeLevel = _sizeLevel;
	
	ofxVec3f gravity(0, 0, 0);
	ofxVec3f center(forcePoint);
	radius = scale;
	radius *= 2.5;
	resolusion = 6000;//ofMap(sizeLevel, YNSOFTMINSIZELEV, YNSOFTMAXSIZELEV, minRes, maxRes);
	yesORno = bullet->createEllipsoid(gravity, center, radius, resolusion);	
	
	col.setColorRadius(1.0);
	col.setColorScale(0.1);
	col.setColorAngle((_yesOrNo == YES) ? -0.34 : -0.02);
	defaultColAng = col.getColorAngle();

	incomingSMSFaceID = 0;
	changeColBySMSRecievedFace(incomingSMSFaceID);

	currColorPointer = destColorPointer;
	prevFaceAngle = 0;
	previousColAng = 0.0;
	
	cout << "faces size = " + ofToString(yesORno->getSoftBody()->m_faces.size()) << endl;

}

void YesNoObjectSoft::clear() {
	addedSMSs.clear();
	//delete yesORno;
}

void YesNoObjectSoft::update() {
	
	updateColorPointer();
	
	btVector3 f = btVector3(0.0, 0.0, 0.0);
	f.setX(ofSignedNoise(ofGetElapsedTimef())*ofRandom(10, 38));
	f.setY(ofSignedNoise(ofGetElapsedTimef())*ofRandom(10, 38));
	f.setZ(ofSignedNoise(ofGetElapsedTimef())*ofRandom(10, 38));	
	yesORno->getSoftBody()->addForce(f);	
	
	if (ofGetFrameNum() % (int)ofRandom(200, 1000) == 0) {
		blowUp(ofRandom(50, 100));
	}
	
}

void YesNoObjectSoft::draw() {
	
	btSoftBody::tNodeArray& nodes(yesORno->getSoftBody()->m_nodes);
	btSoftBody::tLinkArray& links(yesORno->getSoftBody()->m_links);
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);

	vector<float> faceVertexPtr;
	vector<float> faceNormalPtr;
	for(int i = 0; i < faces.size(); i++) {
		btSoftBody::Node* node_0 = faces[i].m_n[0];
		btSoftBody::Node* node_1 = faces[i].m_n[1];
		btSoftBody::Node* node_2 = faces[i].m_n[2];
		faceVertexPtr.push_back(node_0->m_x.getX());
		faceVertexPtr.push_back(node_0->m_x.getY());
		faceVertexPtr.push_back(node_0->m_x.getZ());
		faceVertexPtr.push_back(node_1->m_x.getX());
		faceVertexPtr.push_back(node_1->m_x.getY());
		faceVertexPtr.push_back(node_1->m_x.getZ());		
		faceVertexPtr.push_back(node_2->m_x.getX());
		faceVertexPtr.push_back(node_2->m_x.getY());
		faceVertexPtr.push_back(node_2->m_x.getZ());	
		btVector3 normal0 = node_0->m_n;
		btVector3 normal1 = node_1->m_n;
		btVector3 normal2 = node_2->m_n;		
		faceNormalPtr.push_back(normal0.getX());
		faceNormalPtr.push_back(normal0.getY());
		faceNormalPtr.push_back(normal0.getZ());
		faceNormalPtr.push_back(normal1.getX());
		faceNormalPtr.push_back(normal1.getY());
		faceNormalPtr.push_back(normal1.getZ());
		faceNormalPtr.push_back(normal2.getX());
		faceNormalPtr.push_back(normal2.getY());
		faceNormalPtr.push_back(normal2.getZ());		
	}		
	
	glEnableClientState(GL_VERTEX_ARRAY);		
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
	glEnableClientState(GL_COLOR_ARRAY);	
	glVertexPointer(3, GL_FLOAT, 0, &faceVertexPtr[0]);
	glNormalPointer(GL_FLOAT, 0, &faceNormalPtr[0]);
	glTexCoordPointer(3, GL_FLOAT, 0, &faceVertexPtr[0]);	
	glColorPointer(4, GL_FLOAT, 0, &currColorPointer[0]);	
	glDrawArrays(GL_TRIANGLES, 0, faceVertexPtr.size()/3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	
	glDisableClientState(GL_COLOR_ARRAY);	
	
	for (int i = 0; i < addedSMSs.size(); i++) {
		addedSMS *as = addedSMSs[i];
		
		int fid = as->faceID;
		int cfid = fid*4*3;
		float cr = currColorPointer[cfid];
		float cg = currColorPointer[cfid+1];
		float cb = currColorPointer[cfid+2];
		float ca = currColorPointer[cfid+3];
		glColor4f(cr, cg, cb, ca);

		ofxVec3f fa = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as->faceID].m_n[0]->m_x);
		ofxVec3f fb = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as->faceID].m_n[1]->m_x);
		ofxVec3f fc = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as->faceID].m_n[2]->m_x);
		ofxVec3f a = as->getA();
		ofxVec3f b = as->getB();
		ofxVec3f c = as->getC();
		ofxVec3f norm = as->getNorm();		
		norm *= as->tw.update();
		a += norm;
		b += norm;
		c += norm;
		ofxVec3f cen = getFaceCentroid(faces, fid);
		ofxVec3f an = a-cen;
		ofxVec3f bn = b-cen;
		ofxVec3f cn = c-cen;
		a += an/2;
		b += bn/2;
		c += cn/2;
		//		a -= an/2;
		//		b -= bn/2;
		//		c -= cn/2;		
		ofxQuad(fa, fb, b, a);
		ofxQuad(fb, fc, c, b);
		ofxQuad(fc, fa, a, c);
		ofxTriangleShape(a, b, c);		
		
//		ofxVec3f fa = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as->faceID].m_n[0]->m_x);
//		ofxVec3f fb = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as->faceID].m_n[1]->m_x);
//		ofxVec3f fc = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as->faceID].m_n[2]->m_x);
//		ofxVec3f cen = getFaceCentroid(faces, fid);
//		ofxVec3f norm = as->getNorm();		
//		norm *= as->tw.update();
//		cen += norm;
//		ofxTriangleShape(fb, cen, fa);
//		ofxTriangleShape(fc, cen, fb);
//		ofxTriangleShape(fa, cen, fc);
		
		ofSetColor(255, 255, 255);
	}
	
}

void YesNoObjectSoft::debugDraw() {

	yesORno->render();

}

void YesNoObjectSoft::startFaceingToCam(ofxCamera* cam, ofxVec3f offset) {

	ofxVec3f faceCentroid = getFaceCentroid(yesORno->getSoftBody()->m_faces, incomingSMSFaceID);
	ofxVec3f objCentroid = yesORno->getBodyCentroid();
	ofxVec3f camPos = cam->getPosition()-offset;
	
	ofxVec3f center = objCentroid;
	ofxVec3f tar = camPos;
	ofxVec3f normal = tar - center;
	normal.normalize();
	ofxVec3f forward = faceCentroid - objCentroid;
	forward.normalize();
	ofxVec3f axis = forward.crossed(normal);
	axis.normalize();
	float angle = forward.angle(normal);	
	
	ofxVec3f right = axis.crossed(normal);
	right.normalize();
	right *= 450;
	
	ofxVec3f da = axis;
	da *= 450;
	
	float prev = prevFaceAngle;
	facingTween.setParameters(facingEasing, ofxTween::easeInOut, prevFaceAngle, angle, 2000, 0);
	facingTween.addValue(prevFacingAxis.x, axis.x);
	facingTween.addValue(prevFacingAxis.y, axis.y);
	facingTween.addValue(prevFacingAxis.z, axis.z);	
	ofAddListener(facingTween.end_E, this, &YesNoObjectSoft::addSMSCompleted);
	prevFaceAngle = angle;
	prevFacingAxis = axis;	
	
}

void YesNoObjectSoft::updateRotateion() {

	glRotatef(facingTween.update(), facingTween.getTarget(1), facingTween.getTarget(2), facingTween.getTarget(3));

}

void YesNoObjectSoft::updateColorPointer() {
	
	if (colorPointerTween.isRunning() && !colorPointerTween.isCompleted()) {
		btSoftBody::tFaceArray& faces = yesORno->getSoftBody()->m_faces;
		
		vector<int> sortedFace = sortFaceByDistance(faces, incomingSMSFaceID);
		int farestID = sortedFace[0];
		float farestDist = getFaceDistanceBetween(faces, incomingSMSFaceID, farestID);
		
		vector<float> tmpHolder;
		float factor = colorPointerTween.update();		
		for (int i = 0; i < destColorPointer.size(); i++) {
			float currentColor = currColorPointer[i];
			float destinationColor = destColorPointer[i];
			float colorDiff = currentColor-destinationColor;
			int curFaceID = i/(4*3);
			float faceDistBWSMSandCur = getFaceDistanceBetween(faces, incomingSMSFaceID, curFaceID);
			float mappedDiff = ofMap(faceDistBWSMSandCur, 0.0, farestDist, 0.0, colorDiff);
			float mappedDist = ofMap(faceDistBWSMSandCur, 0.0, farestDist, destinationColor+mappedDiff, currentColor);
			float col = ofMap(factor, 0.0, 100.0, currentColor, mappedDist);			
			tmpHolder.push_back(col);
		}
		currColorPointer.clear();	
		currColorPointer = tmpHolder;
		tmpHolder.clear();
	}
	
}

vector<float> YesNoObjectSoft::changeColBySMSRecievedFace(int z) {

	int faceID = incomingSMSFaceID;
	btSoftBody::tFaceArray& faces = yesORno->getSoftBody()->m_faces;
	vector<int> faceIDVec = sortFaceByDistance(faces, faceID);
	
	vector<float> colPtrRtn;
	for (int i = 0; i < faces.size()*3*4; i++) {
		colPtrRtn.push_back(0.0);
	}
	float * colPtr = &colPtrRtn[0];
	
	int facesSize = faces.size();
	int faceIDVecSize = faceIDVec.size();
	float thisTimeFactor = 0.05;
	if (previousColAng >= 0.25) {
		previousColAng = -0.25;
	}else {
		previousColAng += thisTimeFactor;
	}	
	float thisTimeAngle = previousColAng;

	for (int i = 0; i < faceIDVec.size(); i++) {
		btSoftBody::Node* compnode_0 = faces[faceIDVec[i]].m_n[0];
		btSoftBody::Node* compnode_1 = faces[faceIDVec[i]].m_n[1];
		btSoftBody::Node* compnode_2 = faces[faceIDVec[i]].m_n[2];		

		float scaleFactor = 0.0;
		float radiusFactor = 0.0;
		if (i < faceIDVec.size()/2) {
			scaleFactor = ofMap(i, 0, faceIDVec.size(), 0.001, 0.499);
			radiusFactor = ofMap(i, 0, faceIDVec.size(), 0.001, 0.699);
		}else {
			scaleFactor = ofMap(i, 0, faceIDVec.size(), 0.499, 0.001);
			radiusFactor = ofMap(i, 0, faceIDVec.size(), 0.699, 0.001);					
		}
		
		float angleFactor = defaultColAng+thisTimeAngle;
		float factor = 4;
		if (i > faceIDVec.size()/factor) {
			scaleFactor = ofMap(i, faceIDVec.size()/factor, faceIDVec.size(), 0.1, 0.8);
			radiusFactor = ofMap(i, faceIDVec.size()/factor, faceIDVec.size(), 0.1, 0.8);
		}		
		
//		float scaleFactor = 0.0;
//		float radiusFactor = 0.0;
//		if (i < faceIDVec.size()/2) {
//			scaleFactor = ofMap(i, 0, faceIDVec.size(), 0.001, 0.499);
//			radiusFactor = ofMap(i, 0, faceIDVec.size(), 0.001, 0.699);
//		}else {
//			scaleFactor = ofMap(i, 0, faceIDVec.size(), 0.499, 0.001);
//			radiusFactor = ofMap(i, 0, faceIDVec.size(), 0.699, 0.001);					
//		}
//		
//		float angleFactor = defaultColAng;
//		if (i > faceIDVec.size()/1.04) {
//			scaleFactor = ofMap(i, faceIDVec.size()/1.04, faceIDVec.size(), 0.55, 1.0);
//			radiusFactor = ofMap(i, faceIDVec.size()/1.04, faceIDVec.size(), 0.85, 1.0);
//		}
		
		for (int j = 0; j < 3; j++) {
			int idx = faceIDVec[i]*12+j*4;
			col.setColorAngle(angleFactor);
			col.setColorScale(scaleFactor);
			col.setColorRadius(radiusFactor);
			col.update();
			ofColor tgtc = col.getColor();
			colPtr[idx] = tgtc.r/255.0;
			colPtr[idx+1] = tgtc.g/255.0;
			colPtr[idx+2] = tgtc.b/255.0;
			colPtr[idx+3] = 1.0;					
		}		
	}

	destColorPointer = colPtrRtn;
	return colPtrRtn;	

}

vector<int> YesNoObjectSoft::sortFaceByDistance(btSoftBody::tFaceArray& faces, int faceID) {
	
	
	map<float, int, greater<float> > faceSize;
	ofxVec3f tgtCentroid = getFaceCentroid(faces, faceID);
	for (int i = 0; i < faces.size(); i++) {
		ofxVec3f compCentroid1 = getFaceCentroid(faces, i);
		float d1 = tgtCentroid.distance(compCentroid1);
		faceSize.insert(map<float, int, greater<float> >::value_type(d1, i));
	}
	
	vector<int> faceIDVec;
	map<float, int, greater<float> >::iterator it = faceSize.begin();
	while (it != faceSize.end()) {
		faceIDVec.push_back((*it).second);
		++it;
	}	
	
	return faceIDVec;
}

ofxVec3f YesNoObjectSoft::getFaceCentroid(btSoftBody::tFaceArray& faces, int faceID) {
	
	btSoftBody::Node* node_0 = faces[faceID].m_n[0];
	btSoftBody::Node* node_1 = faces[faceID].m_n[1];
	btSoftBody::Node* node_2 = faces[faceID].m_n[2];		
	
	return ofxVec3f((node_0->m_x.getX()+node_1->m_x.getX()+node_2->m_x.getX())/3,
					(node_0->m_x.getY()+node_1->m_x.getY()+node_2->m_x.getY())/3,
					(node_0->m_x.getZ()+node_1->m_x.getZ()+node_2->m_x.getZ())/3);
	
}

float YesNoObjectSoft::getFaceDistanceBetween(btSoftBody::tFaceArray& faces, int face1ID, int face2ID) {
	
	ofxVec3f face1Centroid = getFaceCentroid(faces, face1ID);
	ofxVec3f face2Centroid = getFaceCentroid(faces, face2ID);
	return face1Centroid.distance(face2Centroid);
	
}

ofxVec3f YesNoObjectSoft::getFaceNormal(btSoftBody::tFaceArray& faces, int faceID) {
	
	btVector3 btNormal = faces[faceID].m_normal;
	return ofxVec3f(btNormal.getX(), btNormal.getY(), btNormal.getZ());
	
}

void YesNoObjectSoft::addSMS(int faceID, int _numSMS, float _ratioSMS) {
	
	incomingSMSFaceID = faceID;
	numSMS = _numSMS;
	ratioSMS = _ratioSMS;
	destColorPointer.clear();
	int i =0;
	changeColBySMSRecievedFace(i);
	colorPointerTween.setParameters(easing, ofxTween::easeIn, 0, 100, 2000, 1500);

}

void YesNoObjectSoft::addSMSCompleted(int & z) {
	
	int faceID = incomingSMSFaceID;
	pinchFace(incomingSMSFaceID);
	
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	
	ofxVec3f a = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[faceID].m_n[0]->m_x);//.rescale(2.0);
	ofxVec3f b = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[faceID].m_n[1]->m_x);//.rescale(2.0);
	ofxVec3f c = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[faceID].m_n[2]->m_x);//.rescale(2.0);				
	
	ofxVec3f norm = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[faceID].m_normal);
	norm *= 50;
	
	addedSMS *as = new addedSMS();
	as->btnormal = &faces[faceID].m_normal;
	as->node0 = faces[faceID].m_n[0];
	as->node1 = faces[faceID].m_n[1];
	as->node2 = faces[faceID].m_n[2];
	float minl = ofMap(resolusion, minRes, maxRes, 10, 40);
	float maxl = ofMap(resolusion, minRes, maxRes, 10, 60);
	as->length = ofRandom(minl, maxl);
	as->faceID = faceID;
	as->angle = ofRandomuf();
	as->tw.setParameters(as->ea, ofxTween::easeIn, 0.0, as->length, 800, 0);
	
	ofAddListener(as->tw.end_E, this, &YesNoObjectSoft::notifyFinishAllUpdating);
	addedSMSs.push_back(as);
	
}

void YesNoObjectSoft::pinchFace(int faceIdx) {
	// get face and nodes
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	btSoftBody::Node* node_0 = faces[faceIdx].m_n[0];
	btSoftBody::Node* node_1 = faces[faceIdx].m_n[1];
	btSoftBody::Node* node_2 = faces[faceIdx].m_n[2];
	
	// pinch face
	int pinchFaceFactor = ofMap(resolusion, minRes, maxRes, 67000, 27700);
	btVector3 normal = faces[faceIdx].m_normal;
	btVector3 pinchFaceForce = normal*pinchFaceFactor;
	
	if (node_0->m_im > 0) node_0->m_f += pinchFaceForce;
	if (node_1->m_im > 0) node_1->m_f += pinchFaceForce;
	if (node_2->m_im > 0) node_2->m_f += pinchFaceForce;	
	
}

void YesNoObjectSoft::blowUp(float pinchFaceFactor) {
	// get face and nodes
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	int fSize = faces.size();
	
	for (int i = 0; i < fSize; i++) {
		
		btVector3 normal = faces[i].m_normal;
		btVector3 pinchFaceForce = normal*pinchFaceFactor;
		
		btSoftBody::Node* node_0 = faces[i].m_n[0];
		btSoftBody::Node* node_1 = faces[i].m_n[1];
		btSoftBody::Node* node_2 = faces[i].m_n[2];	
		if (node_0->m_im > 0) node_0->m_f += pinchFaceForce;
		if (node_1->m_im > 0) node_1->m_f += pinchFaceForce;
		if (node_2->m_im > 0) node_2->m_f += pinchFaceForce;	
		
	}	
}

void YesNoObjectSoft::shrink() {
	// get face and nodes
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	int fSize = faces.size();
	int pinchFaceFactor = ofMap(resolusion, minRes, maxRes, 800, 80);
	
	for (int i = 0; i < fSize; i++) {
		
		btSoftBody::Node* node_0 = faces[i].m_n[0];
		btSoftBody::Node* node_1 = faces[i].m_n[1];
		btSoftBody::Node* node_2 = faces[i].m_n[2];	
		
		// pinch face
		btVector3 normal = -faces[i].m_normal;
		btVector3 pinchFaceForce = normal*pinchFaceFactor;
		
		if (node_0->m_im > 0) node_0->m_f += pinchFaceForce;
		if (node_1->m_im > 0) node_1->m_f += pinchFaceForce;
		if (node_2->m_im > 0) node_2->m_f += pinchFaceForce;		
	}	
}

void YesNoObjectSoft::notifyFinishAllUpdating(int & z) {
	
	int i = 0;
	ofNotifyEvent(onFinishAllUpdating, YesOrNo);
	ofNotifyEvent(notifyScaleYesEvent, i);
	ofNotifyEvent(notifyScaleNoEvent, i);
}
