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
	//radius *= 2.5;
	resolusion = ofMap(sizeLevel, YNSOFTMINSIZELEV, YNSOFTMAXSIZELEV, minRes, maxRes);
	
	float size = 50;
	float verts[12] =  {size,size,size, -size,-size,size, -size,size,-size, size,-size,-size};
	int faces[12] = {0,2,1, 1,3,0, 2,3,1, 0,3,2};
	yesORno = bullet->createSoftTriMesh(ofxVec3f(0, 0, 0), &verts[0], &faces[0], 4);		
	float dist = ofxVec3f(size,size,size).distance(ofxVec3f(-size,-size,size));
	edgeLen = dist;
	height = sqrt(6)/3*dist;
	float x = (size-size-size+size)/4;
	float y = (size-size+size-size)/4;
	float z = (size+size-size-size)/4;
	smsPosition = ofxVec3f(x,y,z);
	
	col.setColorRadius(1.0);
	col.setColorScale(0.2);
	col.setColorAngle((_yesOrNo == YES) ? -0.34 : -0.02);
	defaultColAng = col.getColorAngle();
	
	incomingSMSFaceID = 0;
	changeColBySMSRecievedFace(incomingSMSFaceID);
	
	currColorPointer = destColorPointer;
	prevFaceAngle = 0;
	previousColAng = 0.0;
	suddenMotion = true;
	
	prevFacingAxis = ofxVec3f(0.5635459, -0.7932258, -0.2306707);
	prevFaceAngle = 115.9340439;
	
	sound.loadSound((YesOrNo==0)?"key_omin_a.aif":"A_kick.aif");
	
}

void YesNoObjectSoft::update() {
	
	updateColorPointer();
	
	if (suddenMotion) {	
//		btVector3 f = btVector3(0.0, 0.0, 0.0);
//		f.setX(ofSignedNoise(ofGetElapsedTimef())*ofRandom(10, 38));
//		f.setY(ofSignedNoise(ofGetElapsedTimef())*ofRandom(10, 38));
//		f.setZ(ofSignedNoise(ofGetElapsedTimef())*ofRandom(10, 38));	
//		yesORno->getSoftBody()->addForce(f);	
		
		if (ofGetFrameNum() % (int)ofRandom(20, 100) == 0) {	
			yesORno->blowUp(ofRandom(5000, 10000));		
		}
	}
	
}

void YesNoObjectSoft::updateRotateion() {
	
	from.slerp(quatTween.update(), from, to);
	ofxVec3f axis; float angle;
	from.getRotate(angle, axis);
	glRotatef(ofRadToDeg(angle), axis.x, axis.y, axis.z);
	
}

void YesNoObjectSoft::updateTranslation() {	
	
	ofxVec3f cen = yesORno->getBodyCentroid();
	if (isnan(cen.x) || isnan(cen.y) || isnan(cen.z)) {
		float x = translationTween.getTarget(0);
		float y = translationTween.getTarget(1);
		float z = translationTween.getTarget(2);	
		ofTranslate(-x, -y, -z);	
		return;
	}
	
	objCentroid = cen;	
	if (translationTween.isCompleted()) {
		float x = translationTween.getTarget(0);
		float y = translationTween.getTarget(1);
		float z = translationTween.getTarget(2);
		translationTween.setParameters(translationEasing, ofxTween::easeIn, x, cen.x, 1000, 0);
		translationTween.addValue(y, cen.y);
		translationTween.addValue(z, cen.z);
	}
	float x = translationTween.update();
	float y = translationTween.getTarget(1);
	float z = translationTween.getTarget(2);	
	ofTranslate(-x, -y, -z);	

}

void YesNoObjectSoft::draw() {
	
	btSoftBody::tNodeArray& nodes(yesORno->getSoftBody()->m_nodes);
	btSoftBody::tLinkArray& links(yesORno->getSoftBody()->m_links);
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);

//	ofEnableAlphaBlending();
//	glColor4f(1.0, 1.0, 1.0, 0.4);
//	for(int i = 0; i < faces.size(); i++) {
//		btSoftBody::Node* node_0 = faces[i].m_n[0];
//		btSoftBody::Node* node_1 = faces[i].m_n[1];
//		btSoftBody::Node* node_2 = faces[i].m_n[2];		
//		ofxTriangleShape(
//		node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(),
//		node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ(),		
//		node_2->m_x.getX(), node_2->m_x.getY(), node_2->m_x.getZ()
//						 );		
//	}
//	ofSetLineWidth(3);
//	glColor4f(0.0, 0.0, 0.0, 0.4);
//	for(int i = 0; i < links.size(); i++) {
//		btSoftBody::Node* node_0 = links[i].m_n[0];
//		btSoftBody::Node* node_1 = links[i].m_n[1];
//		ofxLine(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(), 
//				node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ());
//	}	
//	glColor4f(1.0, 0.0, 0.0, 1.0);
//	for (int i = 0; i < smsBaseFace.size()/3; i++) {
//		ofxVec3f a = smsBaseFace[i*3];
//		ofxVec3f b = smsBaseFace[i*3+1];
//		ofxVec3f c = smsBaseFace[i*3+2];	
//		ofxTriangleShape(a, b, c);
//	}
//	ofDisableAlphaBlending();
		
		
	int numNan = 0;
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

	ofEnableSmoothing();
	glEnable(GL_LINE_SMOOTH);
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
		
		vector<ofxVec3f> face = yesORno->getFaceAsVerts(fid);
		ofxVec3f fa = face[0];
		ofxVec3f fb = face[1];
		ofxVec3f fc = face[2];
		ofxVec3f cen = yesORno->getFaceCentroid(fid);
		ofxVec3f norm = yesORno->getFaceNormal(fid);	
		float factor = 0.0;
		if (as->tw.isCompleted()) {
			factor = as->tw.getTarget(0);
		}else {
			factor = as->tw.update();
		}
		norm *= factor;
		cen += norm;
		ofxTriangleShape(fb, cen, fa);
		ofxTriangleShape(fc, cen, fb);
		ofxTriangleShape(fa, cen, fc);
		
		ofSetColor(255, 255, 255);
		
		// store sms faces for pyly gen
		ofxVec3f normc = yesORno->getFaceNormal(fid);
		normc *= height;
		ofxVec3f cenc = yesORno->getFaceCentroid(fid);
		cenc += normc;
		addedSMSFaces.clear();
		addedSMSFaces.push_back(fb);
		addedSMSFaces.push_back(cenc);
		addedSMSFaces.push_back(fa);
		addedSMSFaces.push_back(fc);
		addedSMSFaces.push_back(cenc);
		addedSMSFaces.push_back(fb);
		addedSMSFaces.push_back(fa);
		addedSMSFaces.push_back(cenc);
		addedSMSFaces.push_back(fc);	
		
		// store sms position for color change		
		int i =0;
		changeColBySMSRecievedFace(i);			
		
	}
	
}

void YesNoObjectSoft::startFaceingToCam(ofxCamera* cam, ofxVec3f offset) {
	
	ofxVec3f faceCentroid = yesORno->getFaceCentroid(incomingSMSFaceID);
	ofxVec3f objCentroid = yesORno->getBodyCentroid();
	ofxVec3f camPos = cam->getPosition()-offset;
	
	ofxVec3f center = objCentroid;
	ofxVec3f tar = camPos;
	ofxVec3f normal = tar - center;
//	ofxVec3f normal = center - tar;	
	normal.normalize();
//	ofxVec3f forward = faceCentroid - objCentroid;
	ofxVec3f forward = objCentroid - faceCentroid;	
	forward.normalize();	
//	ofxVec3f axis = forward.crossed(normal);
	ofxVec3f axis = normal.crossed(forward);	
	axis.normalize();
//	float angle = forward.angle(normal);
	float angle = normal.angle(forward);	
	
	quatTween.setParameters(quatEasing, ofxTween::easeIn, 0.0, 1.0, 950, 0);
	ofAddListener(quatTween.end_E, this, &YesNoObjectSoft::addSMSCompleted);
	from = ofxQuaternion(prevFacingAxis.x, prevFacingAxis.y, prevFacingAxis.z, ofDegToRad(prevFaceAngle));
	to = ofxQuaternion(axis.x, axis.y, axis.z, ofDegToRad(angle));

//	cout << "prev angle = "+ofToString(prevFaceAngle) << endl;
//	cout << "prev axis = "+ofToString(prevFacingAxis.x)+" "+ofToString(prevFacingAxis.y)+" "+ofToString(prevFacingAxis.z) << endl;		
//	cout << "angle = "+ofToString(angle) << endl;
//	cout << "axis = "+ofToString(axis.x)+" "+ofToString(axis.y)+" "+ofToString(axis.z) << endl;	
	
	prevFaceAngle = angle;
	prevFacingAxis = axis;		
	
	ofNotifyEvent(notifyStartCamOrbit, YesOrNo);
}

void YesNoObjectSoft::updateColorPointer() {
	
	if (colorPointerTween.isRunning() && !colorPointerTween.isCompleted()) {
		btSoftBody::tFaceArray& faces = yesORno->getSoftBody()->m_faces;
		
		vector<int> sortedFace = yesORno->sortFaceByPosition(smsPosition);		
		int farestID = sortedFace[0];
		float farestDist = yesORno->getFaceDistanceBetween(sortedFace[sortedFace.size()-1], farestID);		
		
		vector<float> tmpHolder;
		float factor = colorPointerTween.update();		
		for (int i = 0; i < destColorPointer.size(); i++) {
			float currentColor = currColorPointer[i];
			float destinationColor = destColorPointer[i];
			float colorDiff = currentColor-destinationColor;
			int curFaceID = i/(4*3);
			float faceDistBWSMSandCur = yesORno->getFaceDistanceBetween(sortedFace[sortedFace.size()-1], curFaceID);
			float mappedDiff = ofMap(faceDistBWSMSandCur, 0.0, farestDist, 0.0, colorDiff);
			float mappedDist = ofMap(faceDistBWSMSandCur, 0.0, farestDist, destinationColor+mappedDiff, currentColor);
			float col = ofMap(factor, 0.0, 100.0, currentColor, mappedDist);			
			tmpHolder.push_back(col);
		}
		currColorPointer.clear();	
		currColorPointer = tmpHolder;
		currColorPointer = destColorPointer;
		tmpHolder.clear();
	}
	
}

vector<float> YesNoObjectSoft::changeColBySMSRecievedFace(int z) {
	
	int faceID = incomingSMSFaceID;
	btSoftBody::tFaceArray& faces = yesORno->getSoftBody()->m_faces;
	vector<int> faceIDVec = yesORno->sortFaceByPosition(smsPosition);	
	
	vector<float> colPtrRtn;
	for (int i = 0; i < faces.size()*3*4; i++) {
		colPtrRtn.push_back(0.0);
	}
	float * colPtr = &colPtrRtn[0];
	
	float thisTimeFactor = 0.05;
	if (previousColAng >= 0.25) {
		previousColAng = -0.25;
	}else {
		previousColAng += thisTimeFactor;
	}	
	float thisTimeAngle = previousColAng;
	
	for (int i = 0; i < faceIDVec.size(); i++) {		
		
		float scaleFactor = 0.0;
		float radiusFactor = 0.0;
		if (i < faceIDVec.size()/2) {
			scaleFactor = ofMap(i, 0, faceIDVec.size(), 0.1, 0.499);
			radiusFactor = ofMap(i, 0, faceIDVec.size(), 0.001, 0.699);
		}else {
			scaleFactor = ofMap(i, 0, faceIDVec.size(), 0.499, 0.1);
			radiusFactor = ofMap(i, 0, faceIDVec.size(), 0.699, 0.001);					
		}
		
		float angleFactor = defaultColAng+thisTimeAngle;
		float factor = 4;
		if (i > faceIDVec.size()/factor) {
			scaleFactor = ofMap(i, faceIDVec.size()/factor, faceIDVec.size(), 0.1, 0.8);
			radiusFactor = ofMap(i, faceIDVec.size()/factor, faceIDVec.size(), 0.1, 0.8);
		}		
		
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
			colPtr[idx+3] = 0.2;					
		}
		
	}
	
	destColorPointer = colPtrRtn;
	return colPtrRtn;	
	
}

void YesNoObjectSoft::addSMS(int faceID, int _numSMS, float _ratioSMS) {
	
	incomingSMSFaceID = faceID;
	numSMS = _numSMS;
	ratioSMS = _ratioSMS;
	destColorPointer.clear();
	
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	int max = faces.size();
	colorPointerTween.setParameters(easing, ofxTween::easeIn, 0, 100, 2000, 1500);
	
}

void YesNoObjectSoft::addSMSCompleted(int & z) {
	
	suddenMotion = false;	
	
	// store sms position for color change		
	ofxVec3f normc = yesORno->getFaceNormal(incomingSMSFaceID);
	normc *= height;
	ofxVec3f cenc = yesORno->getFaceCentroid(incomingSMSFaceID);
	cenc += normc;
	smsPosition = ofxVec3f(cenc.x, cenc.y, cenc.z);
	int i =0;
	changeColBySMSRecievedFace(i);		
	
	int faceID = incomingSMSFaceID;
	yesORno->pinchFace(incomingSMSFaceID, ofMap(resolusion, minRes, maxRes, 67000, 27700));
	
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
	float minl = ofMap(resolusion, minRes, maxRes, 140, 70);
	float maxl = ofMap(resolusion, minRes, maxRes, 160, 120);
	as->length = height;//sqrt(6)/2*(a.distance(b));//sqrt(6)*100/3;//ofRandom(minl, maxl);
	as->faceID = faceID;
	as->angle = ofRandomuf();
	as->tw.setParameters(as->ea, ofxTween::easeOut, 0.0, as->length, 800, 0);
	as->tw.id = ofGetFrameNum();
	
	ofAddListener(as->tw.end_E, this, &YesNoObjectSoft::notifyFinishAllUpdating);
	ofAddListener(as->endSpikeGlow, this, &YesNoObjectSoft::onEndSpikeGlow);
	addedSMSs.push_back(as);
	
	ofNotifyEvent(notifyScaleYesEvent, i);
	ofNotifyEvent(notifyScaleNoEvent, i);
	
	sound.play();
	
}

void YesNoObjectSoft::notifyFinishAllUpdating(int & z) {
	
	delete addedSMSs[addedSMSs.size()-1];
	addedSMSs.clear();	
	
	ofNotifyEvent(onFinishAllUpdating, YesOrNo);
	
}

void YesNoObjectSoft::onEndSpikeGlow(int & z) {
	
	fillMeshInput();
	
	cout << "-----------------------------------------------" << endl;
	cout << "floatVertices size = " + ofToString((int)floatVertices.size()) << endl;
	cout << "faceIndices size = " + ofToString((int)faceIndices.size()) << endl;
	cout << "numFace = " + ofToString(numFace) << endl;
	
	yesORno->remove(bullet->getSoftDynamicsWorld());
	delete yesORno;
	yesORno = bullet->createSoftTriMesh(ofxVec3f(0, 0, 0), &floatVertices[0], &faceIndices[0], numFace);		
	
	cout << "-----------------------------------------------" << endl;
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	cout << "generated face size = " + ofToString((int)faces.size()) << endl;
	cout << "generated verts size = " + ofToString((int)faces.size()*3) << endl;
//	for(int i = 0; i < faces.size(); i++) {
//		btSoftBody::Node* node_0 = faces[i].m_n[0];
//		btSoftBody::Node* node_1 = faces[i].m_n[1];
//		btSoftBody::Node* node_2 = faces[i].m_n[2];
//		ofxVec3f a = ofxVec3f(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ());
//		ofxVec3f b = ofxVec3f(node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ());		
//		ofxVec3f c = ofxVec3f(node_2->m_x.getX(), node_2->m_x.getY(), node_2->m_x.getZ());
//		cout << "a = "+ofToString(a.x)+" "+ofToString(a.y)+" "+ofToString(a.z) << endl;
//		cout << "b = "+ofToString(b.x)+" "+ofToString(b.y)+" "+ofToString(b.z) << endl;
//		cout << "c = "+ofToString(c.x)+" "+ofToString(c.y)+" "+ofToString(c.z) << endl;		
//	}
	cout << " " << endl;	
	
	addedSMSFaces.clear();
	suddenMotion = true;
		
}

void YesNoObjectSoft::fillMeshInput() {
	
	vector<ofxVec3f> rawFacesAsVerts = yesORno->getAllFacesAsVerts();
	
	// remove base face under incomingSMS
	vector< vector<ofxVec3f> > faces;
	for (int i = 0; i < rawFacesAsVerts.size()/3; i++) {
		ofxVec3f v0 = rawFacesAsVerts[i*3];
		ofxVec3f v1 = rawFacesAsVerts[i*3+1];
		ofxVec3f v2 = rawFacesAsVerts[i*3+2];
		vector<ofxVec3f> face;
		face.push_back(v0);
		face.push_back(v1);
		face.push_back(v2);
		faces.push_back(face);
	}
	vector<ofxVec3f> baseFace = yesORno->getFaceAsVerts(incomingSMSFaceID);
	for (int i = 0; i < faces.size(); i++) {
		vector<ofxVec3f> compFace = faces[i];
		ofxVec3f a = compFace[0];
		ofxVec3f ca = compFace[1];
		ofxVec3f b = compFace[2];
		ofxVec3f cb = baseFace[0];
		ofxVec3f c = baseFace[1];
		ofxVec3f cc = baseFace[2];
		if (a == ca && b == cb && c == cc) {
			faces.erase(faces.begin()+i);
		}else if (a == ca) {
			if (b == cb && c == cc) {
				faces.erase(faces.begin()+i);
			}else if (b == cc && c == cb) {
				faces.erase(faces.begin()+i);		
			}
		}else if (b == ca) {
			if (a == cb && c == cc) {
				faces.erase(faces.begin()+i);
			}else if (a == cc && c == cb) {
				faces.erase(faces.begin()+i);
			}
		}else if (c == ca) {
			if (a == cb && b == cc) {
				faces.erase(faces.begin()+i);
			}else if (a == cc && b == cb) {
				faces.erase(faces.begin()+i);				
			}
		}		
	}
	
	// convert array geometory
	rawFacesAsVerts.clear();
	for (int i = 0; i < faces.size(); i++) {
		vector<ofxVec3f> face = faces[i];
		ofxVec3f v0 = face[0];
		ofxVec3f v1 = face[1];
		ofxVec3f v2 = face[2];
		rawFacesAsVerts.push_back(v0);
		rawFacesAsVerts.push_back(v1);
		rawFacesAsVerts.push_back(v2);
	}
	
	// merge baseFace and addedSMSFaces verts to keep hull as one body
	for (int i = 0; i < addedSMSFaces.size(); i++) {
		ofxVec3f asfV = addedSMSFaces[i];
		for (int j = 0; j < baseFace.size(); j++) {
			ofxVec3f bfV = baseFace[j];
			if (10 > asfV.distance(bfV)) {
				addedSMSFaces[i] = bfV;
				break;
			}
		}
	}	
	
	// add incoming SMS faces to body verts
	for (int i = 0; i < addedSMSFaces.size(); i++) {
		rawFacesAsVerts.push_back(addedSMSFaces[i]);
	}
	
	// remove duplicated verts
	vector<ofxVec3f> vertices = rawFacesAsVerts;
	for (int i = 0; i < rawFacesAsVerts.size(); i++) {
		ofxVec3f v = rawFacesAsVerts[i];
		int cnt = 0;
		for (int j = 0; j < vertices.size(); j++) {
			ofxVec3f jv = vertices[j];
			if (v == jv) {// || edgeLen*0.4 > v.distance(jv)) {
				if (cnt > 0) {
					vertices.erase(vertices.begin()+j);
				}
				cnt++;					
			}
		}
	}	
	
	// raw faces
	vector<int> rawFaceIndices;
	for (int i = 0; i < rawFacesAsVerts.size(); i++) {
		ofxVec3f tgt = rawFacesAsVerts[i];
		for (int j = 0; j < vertices.size(); j++) {
			ofxVec3f comp = vertices[j];
			if (tgt == comp) {
				rawFaceIndices.push_back(j);
				break;
			}
		}
	}	
//	cout << "----raw faces "+ofToString((int)rawFaceIndices.size()/3)+" -----" << endl;	
	for (int i = 0; i < rawFaceIndices.size()/3; i++) {
		int v0 = rawFaceIndices[i*3];
		int v1 = rawFaceIndices[i*3+1];
		int v2 = rawFaceIndices[i*3+2];
//		cout << ofToString(v0)+" "+ofToString(v1)+" "+ofToString(v2) << endl;	
	}	
	
	
	// create face indices
	faceIndices.clear();
	for (int i = 0; i < rawFacesAsVerts.size(); i++) {
		ofxVec3f tgt = rawFacesAsVerts[i];
		for (int j = 0; j < vertices.size(); j++) {
			ofxVec3f comp = vertices[j];
			if (tgt == comp || edgeLen*0.6 > tgt.distance(comp)) {
				faceIndices.push_back(j);
				break;
			}
		}
	}
	
	
	int fff = faceIndices.size();
//	cout << "----extra face before remove duplication "+ofToString(fff%3)+" ----" << endl;
	for (int i = 0; i < fff%3; i++) {
		int idx = faceIndices[faceIndices.size()-i-1];
//		cout << ofToString(idx)+" ";
	}
//	cout << " " << endl;	
	for (int i = 0; i < fff%3; i++) {
		faceIndices.pop_back();
	}	
	
//	cout << "----face before remove sequencial loop "+ofToString((int)faceIndices.size()/3)+" -----" << endl;	
	for (int i = 0; i < faceIndices.size()/3; i++) {
		int v0 = faceIndices[i*3];
		int v1 = faceIndices[i*3+1];
		int v2 = faceIndices[i*3+2];
//		cout << ofToString(v0)+" "+ofToString(v1)+" "+ofToString(v2) << endl;	
	}	
	
	// remove sequencial loop face
	vector<int> clearFaceIndice = faceIndices;
	int cnt = 0;	
	for (int i = 0; i < faceIndices.size()/3; i++) {
		int v0 = faceIndices[i*3];
		int v1 = faceIndices[i*3+1];
		int v2 = faceIndices[i*3+2];
		if (v0 == v1 || v1 == v2 || v2 == v0) {
			clearFaceIndice.erase(clearFaceIndice.begin()+i*3-cnt, clearFaceIndice.begin()+i*3+3-cnt);
//			cout << "remove seq faces" << endl;
			cnt+=3;
		}
	}
	faceIndices.clear();
	faceIndices = clearFaceIndice;	
	
//	cout << "----face before remove duplicated face "+ofToString((int)faceIndices.size()/3)+" -----" << endl;	
	for (int i = 0; i < faceIndices.size()/3; i++) {
		int v0 = faceIndices[i*3];
		int v1 = faceIndices[i*3+1];
		int v2 = faceIndices[i*3+2];
//		cout << ofToString(v0)+" "+ofToString(v1)+" "+ofToString(v2) << endl;	
	}		
	
	// remove duplicated faces
	vector<int> cleanFaces = faceIndices;
	for (int i = 0; i < faceIndices.size()/3; i++) {
		int a = faceIndices[i*3];
		int b = faceIndices[i*3+1];
		int c = faceIndices[i*3+2];
		int cnt = 0;
		for (int j = 0; j < cleanFaces.size()/3; j++) {
			int ca = cleanFaces[j*3];
			int cb = cleanFaces[j*3+1];
			int cc = cleanFaces[j*3+2];			
			if (a == ca && b == cb && c == cc) {
				if (cnt > 0) {
					cleanFaces.erase(cleanFaces.begin()+j*3, cleanFaces.begin()+j*3+3);
				}
				cnt++;				
			}
		}
	}
	faceIndices.clear();
	faceIndices = cleanFaces;
	fff = faceIndices.size();
//	cout << "----extra face after remove duplication "+ofToString(fff%3)+" ----" << endl;	
	for (int i = 0; i < fff%3; i++) {
		int idx = faceIndices[faceIndices.size()-i-1];
//		cout << ofToString(idx)+" ";
	}
//	cout << " " << endl;	
	for (int i = 0; i < fff%3; i++) {
		faceIndices.pop_back();
	}
	
//	cout << "----face before remove geometorical identity "+ofToString((int)faceIndices.size()/3)+" -----" << endl;	
	for (int i = 0; i < faceIndices.size()/3; i++) {
		int v0 = faceIndices[i*3];
		int v1 = faceIndices[i*3+1];
		int v2 = faceIndices[i*3+2];
//		cout << ofToString(v0)+" "+ofToString(v1)+" "+ofToString(v2) << endl;	
	}	
	
	// merge faces which share same geometory
	for (int i = 0; i < faceIndices.size()/3; i++) {
		int a = faceIndices[i*3];
		int b = faceIndices[i*3+1];
		int c = faceIndices[i*3+2];
		int cnt = 0;
		for (int j = 0; j < cleanFaces.size()/3; j++) {
			int ca = cleanFaces[j*3];
			int cb = cleanFaces[j*3+1];
			int cc = cleanFaces[j*3+2];			
			if (a == ca) {
				if (b == cb && c == cc) {
					if (cnt > 0) {
						cleanFaces.erase(cleanFaces.begin()+j*3, cleanFaces.begin()+j*3+3);
					}
					cnt++;						
				}else if (b == cc && c == cb) {
					if (cnt > 0) {
						cleanFaces.erase(cleanFaces.begin()+j*3, cleanFaces.begin()+j*3+3);
					}
					cnt++;						
				}
			}else if (b == ca) {
				if (a == cb && c == cc) {
					if (cnt > 0) {
						cleanFaces.erase(cleanFaces.begin()+j*3, cleanFaces.begin()+j*3+3);
					}
					cnt++;					
				}else if (a == cc && c == cb) {
					if (cnt > 0) {
						cleanFaces.erase(cleanFaces.begin()+j*3, cleanFaces.begin()+j*3+3);
					}
					cnt++;	
				}
			}else if (c == ca) {
				if (a == cb && b == cc) {
					if (cnt > 0) {
						cleanFaces.erase(cleanFaces.begin()+j*3, cleanFaces.begin()+j*3+3);
					}
					cnt++;					
				}else if (a == cc && b == cb) {
					if (cnt > 0) {
						cleanFaces.erase(cleanFaces.begin()+j*3, cleanFaces.begin()+j*3+3);
					}
					cnt++;					
				}
			}
		}
	}
	faceIndices.clear();
	faceIndices = cleanFaces;	
	smsBaseFace = baseFace;	
	
	fff = faceIndices.size();
//	cout << "----extra face after merge geometory "+ofToString(fff%3)+" ----" << endl;
	for (int i = 0; i < fff%3; i++) {
		int idx = faceIndices[faceIndices.size()-i-1];
//		cout << ofToString(idx)+" ";
	}
//	cout << " " << endl;
	for (int i = 0; i < fff%3; i++) {
		faceIndices.pop_back();
	}	
	
	
//	cout << "----face "+ofToString((int)faceIndices.size()/3)+" -----" << endl;	
	for (int i = 0; i < faceIndices.size()/3; i++) {
		int v0 = faceIndices[i*3];
		int v1 = faceIndices[i*3+1];
		int v2 = faceIndices[i*3+2];
//		cout << ofToString(v0)+" "+ofToString(v1)+" "+ofToString(v2) << endl;	
	}
//	cout << "----verts "+ofToString((int)vertices.size())+" ----" << endl;
	for (int i = 0; i < vertices.size(); i++) {
		ofxVec3f v = vertices[i];
//		cout << ofToString(v.x)+" "+ofToString(v.y)+" "+ofToString(v.z) << endl;	
	}
		
	
	// create float array vertices
	floatVertices.clear();
	for (int i = 0; i < vertices.size(); i++) {
		ofxVec3f v = vertices[i];
		floatVertices.push_back(v.x);
		floatVertices.push_back(v.y);
		floatVertices.push_back(v.z);
	}
		
	numFace = faceIndices.size()/3;
	
}

void YesNoObjectSoft::clear() {
	addedSMSs.clear();
	//delete yesORno;
}

void YesNoObjectSoft::debugDraw() {
	
	yesORno->render();
	
}
