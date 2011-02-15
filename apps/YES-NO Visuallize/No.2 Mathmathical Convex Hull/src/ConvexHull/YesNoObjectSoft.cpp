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
	maxValu = 0.01;
	sizeLevel = _sizeLevel;
	
	ofxVec3f gravity(0, 0, 0);
	ofxVec3f center(forcePoint);
	radius = scale;
	radius *= 2.5;
	resolusion = ofMap(sizeLevel, YNSOFTMINSIZELEV, YNSOFTMAXSIZELEV, minRes, maxRes);
	yesORno = bullet->createEllipsoid(gravity, center, radius, resolusion);	
	
	col.setColorRadius(1.0);
	col.setColorScale(0.25);
	col.setColorAngle(0.1);
	
	changeColBySMSRecievedFace(yesORno->getSoftBody()->m_faces, 0);
	incomingSMSFaceID = 0;
	currColorPointer = destColorPointer;
	
	bFinish = false;
	blowUpTimerr = new TimedCounter(1);	
	
}

void YesNoObjectSoft::clear() {
	addedSMSs.clear();
	delete yesORno;
}

void YesNoObjectSoft::update() {
	
	updateColorPointer();
		
	btVector3 f = btVector3(0.0, 0.0, 0.0);
	f.setY(ofSignedNoise(ofGetElapsedTimef())*500);
	yesORno->getSoftBody()->addForce(f);
	
//	btQuaternion q;
//	q.setRotation(ofxBulletStaticUtil::ofxVec3ToBtVec3(ofxVec3f(0,1,0)), ofxBulletStaticUtil::degToRad(ofNoise(ofGetFrameNum())));	
//	yesORno->getSoftBody()->rotate(q);
	
	if (bFinish) {
		if (shrinkTimer.isRunning()) {
			shrink();
			shrinkTimer.update();
		}
		if (shrinkTimer.isCompleted()) {
			blowUpTimerr->startCount();
			blowUpTimer.setParameters(easing, ofxTween::easeIn, 0, 100, 1000, 0);

//			if (blowUpTimer.isRunning()) {
//				blowUp();
//				blowUpTimer.update();
//			}
//			if (blowUpTimer.isCompleted()) {
//				ofNotifyEvent(onTheEnd, YesOrNo);
//			}			
		}
		if (blowUpTimerr->isCounting()) {
			blowUp();
			blowUpTimerr->update();
		}		
		if (blowUpTimerr->isCountComplete()) {
			ofNotifyEvent(onTheEnd, YesOrNo);
		}
	}
}

void YesNoObjectSoft::draw() {
	
	btSoftBody::tNodeArray& nodes(yesORno->getSoftBody()->m_nodes);
	btSoftBody::tLinkArray& links(yesORno->getSoftBody()->m_links);
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	
//	ofSetColor(0, 0, 255);
//	for(int i = 0; i < links.size(); i++) {
//		btSoftBody::Node* node_0 = links[i].m_n[0];
//		btSoftBody::Node* node_1 = links[i].m_n[1];
//		ofxLine(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(), 
//				node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ());
//	}		
	
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
	
	
	//glDisable(GL_LIGHTING);
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

//		ofxVec3f fa = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as.faceID].m_n[0]->m_x);
//		ofxVec3f fb = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as.faceID].m_n[1]->m_x);
//		ofxVec3f fc = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[as.faceID].m_n[2]->m_x);
//		ofxVec3f a = as.getA();
//		ofxVec3f b = as.getB();
//		ofxVec3f c = as.getC();
//		ofxVec3f norm = as.getNorm();		
//		norm *= as.length;
//		col.setColorAngle(as.angle);
//		col.setColorScale(0.0);
//		col.update();
//		ofColor clr = col.getColor();
//		ofSetColor(clr.r, clr.g, clr.b);
//		ofxLine(a+norm, b+norm);
//		ofxLine(b+norm, c+norm);
//		ofxLine(c+norm, a+norm);
//		ofxLine(a+norm, fa);
//		ofxLine(b+norm, fb);
//		ofxLine(c+norm, fc);
//		ofxVec3f normb = as.getNorm();	
//		normb *= 2;
//		ofxLine(fa+normb, fb+normb);
//		ofxLine(fb+normb, fc+normb);
//		ofxLine(fc+normb, fa+normb);
		
		ofSetColor(255, 255, 255);
	}
	//glEnable(GL_LIGHTING);	
	
}

void YesNoObjectSoft::debugDraw() {

	yesORno->render();

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

vector<float> YesNoObjectSoft::changeColBySMSRecievedFace(btSoftBody::tFaceArray& faces, int faceID) {

	vector<int> faceIDVec = sortFaceByDistance(faces, faceID);
	
	vector<float> colPtrRtn;
	for (int i = 0; i < faces.size()*3*4; i++) {
		colPtrRtn.push_back(0.0);
	}
	float * colPtr = &colPtrRtn[0];
	
	float angle = 0.0;
	float angleStep = 0.005;//1.0/faces.size();
	float scale = 0.0;
	float scaleStep = 0.005;//1.0/(faces.size()*4);
	int facesSize = faces.size();
	int faceIDVecSize = faceIDVec.size();
	for (int i = 0; i < faceIDVec.size(); i++) {
		btSoftBody::Node* compnode_0 = faces[faceIDVec[i]].m_n[0];
		btSoftBody::Node* compnode_1 = faces[faceIDVec[i]].m_n[1];
		btSoftBody::Node* compnode_2 = faces[faceIDVec[i]].m_n[2];		
		
		for (int j = 0; j < 3; j++) {
			
			int idx = faceIDVec[i]*12+j*4;
			col.setColorAngle(angle);
			col.setColorScale(scale);
			col.update();
			ofColor c = col.getColor();
			colPtr[idx] = c.r/255.0;
			colPtr[idx+1] = c.g/255.0;
			colPtr[idx+2] = c.b/255.0;
			colPtr[idx+3] = 1.0;					
		}		
		
		angle += angleStep;
		scale += scaleStep;
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

void YesNoObjectSoft::debugKeyPressed(int key) {

	btVector3 f;
	switch(key) {
		case 'w':
			f.setX(0);
			f.setY(-10000);
			f.setZ(0);		
			yesORno->getSoftBody()->addForce(f);
			break;
		case 'a':
			f.setX(-10000);
			f.setY(0);
			f.setZ(0);		
			yesORno->getSoftBody()->addForce(f);
			break;
		case 's':
			f.setX(0);
			f.setY(10000);
			f.setZ(0);		
			yesORno->getSoftBody()->addForce(f);
			break;
		case 'd':
			f.setX(10000);
			f.setY(0);
			f.setZ(0);		
			yesORno->getSoftBody()->addForce(f);
			break;
		case 'z':
			btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
			int fSize = faces.size();
			int rdmF = ofRandom(0, fSize);
			pinchFace(rdmF);
			break;

	}
	
	if (key == 'm') {
		yesORno->getSoftBody()->scale(btVector3(2.0, 2.0, 2.0
												));
	}
	if (key == 'b') {
		blowUp();
	}
	if (key == 'x') {
		shrink();
	}
	if (key == 'i') {
		//addSMS();
	}
	if (key == 'p') {

		btSoftBody::tNodeArray& nodes(yesORno->getSoftBody()->m_nodes);
		vector<btVector3> ns;
		for (int i = 0; i < nodes.size(); i++) {
			ns.push_back(nodes[i].m_x);
		}
		ns.push_back(btVector3(100, 100, 100));
		
		yesORno = bullet->createSoftConvexHull(ofxVec3f(0, 80, 0), &ns[0], nodes.size()+1);
		
	}
	if (key == 'g') {
		yesORno->softBodyWI.m_gravity.setValue(0, 10, 0);
	}
	
	if (key == 'l') {
		int faceIdx = 0;
		btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
		btSoftBody::Node* node_0 = faces[faceIdx].m_n[0];
		btSoftBody::Node* node_1 = faces[faceIdx].m_n[1];
		btSoftBody::Node* node_2 = faces[faceIdx].m_n[2];
		
		ofxVec3f centroid = getFaceCentroid(faces, faceIdx);
		ofxVec3f node0 = ofxBulletStaticUtil::btVec3ToOfxVec3(node_0->m_x);
		ofxVec3f node1 = ofxBulletStaticUtil::btVec3ToOfxVec3(node_1->m_x);
		ofxVec3f node2 = ofxBulletStaticUtil::btVec3ToOfxVec3(node_2->m_x);		
		
		ofxVec3f dir0 = node0-centroid;
		ofxVec3f dir1 = node1-centroid;
		ofxVec3f dir2 = node2-centroid;
		
		float factor = 10;
		node_0->m_battach = 0;
		node_1->m_battach = 0;
		node_2->m_battach = 0;		
		node_0->m_x += ofxBulletStaticUtil::ofxVec3ToBtVec3(dir0) *factor;
		node_1->m_x += ofxBulletStaticUtil::ofxVec3ToBtVec3(dir1) *factor;
		node_2->m_x += ofxBulletStaticUtil::ofxVec3ToBtVec3(dir2) *factor;
		
//		if (node_0->m_im > 0) node_0->m_f += ofxBulletStaticUtil::ofxVec3ToBtVec3(dir0) *factor;
//		if (node_1->m_im > 0) node_1->m_f += ofxBulletStaticUtil::ofxVec3ToBtVec3(dir1) *factor;
//		if (node_2->m_im > 0) node_2->m_f += ofxBulletStaticUtil::ofxVec3ToBtVec3(dir2) *factor;
	}
}

void YesNoObjectSoft::addSMS(int faceID) {
	
	incomingSMSFaceID = faceID;
	pinchFace(faceID);
	destColorPointer.clear();
	changeColBySMSRecievedFace(yesORno->getSoftBody()->m_faces, faceID);
	colorPointerTween.setParameters(easing, ofxTween::easeIn, 0, 100, 2000, 0);

}

void YesNoObjectSoft::addSMSCompleted(int faceID, vector<ofxVec3f> pos) {
		
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
	float maxl = ofMap(resolusion, minRes, maxRes, 100, 450);
	as->length = ofRandom(minl, maxl);
	as->faceID = faceID;
	as->angle = ofRandomuf();
	as->tw.setParameters(as->ea, ofxTween::easeIn, 0.0, as->length, 200, 0);
	addedSMSs.push_back(as);
	
//	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
//	btSoftBody::tNodeArray& nodes(yesORno->getSoftBody()->m_nodes);
//	
//	// 1.allign all triangle(face) refs to verts idx
//	vector<vector<int> > vertsIdx;
//	for (int i = 0; i < faces.size(); i++) {
//		btSoftBody::Node* node0 = faces[i].m_n[0];
//		btSoftBody::Node* node1 = faces[i].m_n[1];
//		btSoftBody::Node* node2 = faces[i].m_n[2];	
//		btVector3 n0p = node0->m_x;
//		btVector3 n1p = node1->m_x;
//		btVector3 n2p = node2->m_x;
//		// get each idx of node
//		int idxN0 = 0; int idxN1 = 0; int idxN2 = 0;
//		bool n0 = false; bool n1 = false; bool n2 = false;		
//		for (int j = 0; j < nodes.size(); j++) {
//			btSoftBody::Node compNode = nodes[j];
//			btVector3 cnp = compNode.m_x;
//			if (n0p == cnp) {
//				idxN0 = j; n0 = true;
//			}
//			if (n1p == cnp) {
//				idxN1 = j; n1 = true;
//			}
//			if (n2p == cnp) {
//				idxN2 = j; n2 = true;
//			}
//			if (n0 && n1 && n2) break;
//		}
//		vector<int> faceNodeRef;
//		faceNodeRef.push_back(idxN0);
//		faceNodeRef.push_back(idxN1);
//		faceNodeRef.push_back(idxN2);		
//		vertsIdx.push_back(faceNodeRef);
//	}
//	for (int i = 0; i < vertsIdx.size(); i++) {
//		vector<int> faceNodeRef = vertsIdx[i];
//		cout << "{" ;
//		for (int j = 0; j < faceNodeRef.size(); j++) {
//			int out = faceNodeRef[j];
//			cout << ofToString(out) + ", ";
//		}
//		cout << "}" << endl;
//	}
//	cout << "" << endl;
//	
//	// 2.get verts pos
//	vector<btScalar> vertices; 
//	for (int i = 0; i < nodes.size(); i++) {
//		btSoftBody::Node n = nodes[i];
//		btVector3 pos = n.m_x;		
//		vertices.push_back(pos.getX());
//		vertices.push_back(pos.getY());
//		vertices.push_back(pos.getZ());
//	}
//	
//	yesORno = bullet->createSoftTriMesh(ofxVec3f(0, 80, 0), &vertices[0], &vertsIdx[0][0], vertsIdx.size());
//	
	
//	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
//	vector<btVector3> ns;
//	vector<ofxVec3f> comp;
//	for (int i = 0; i < faces.size(); i++) {
//		if (i != faceID) {
//			btSoftBody::Node* node_0 = faces[i].m_n[0];
//			btSoftBody::Node* node_1 = faces[i].m_n[1];
//			btSoftBody::Node* node_2 = faces[i].m_n[2];
//			
//			bool aPush = true;
//			bool bPush = true;
//			bool cPush = true;
//			for (int j = 0; j < comp.size(); j++) {
//				ofxVec3f c = comp[j];
//				if (c == ofxBulletStaticUtil::btVec3ToOfxVec3(node_0->m_x)) {
//					aPush = false;
//				}else {
//					comp.push_back(ofxBulletStaticUtil::btVec3ToOfxVec3(node_0->m_x));
//				}
//				if (c == ofxBulletStaticUtil::btVec3ToOfxVec3(node_1->m_x)) {
//					bPush = false;
//				}else {
//					comp.push_back(ofxBulletStaticUtil::btVec3ToOfxVec3(node_1->m_x));
//				}
//				if (c == ofxBulletStaticUtil::btVec3ToOfxVec3(node_2->m_x)) {
//					cPush = false;
//				}else {
//					comp.push_back(ofxBulletStaticUtil::btVec3ToOfxVec3(node_2->m_x));
//				}				
//			}
//			
//			if (aPush) ns.push_back(node_0->m_x);
//			if (bPush) ns.push_back(node_1->m_x);
//			if (cPush) ns.push_back(node_2->m_x);
//			
//		}else {
//				
//			ofxVec3f a = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[i].m_n[0]->m_x);//.rescale(2.0);
//			ofxVec3f b = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[i].m_n[1]->m_x);//.rescale(2.0);
//			ofxVec3f c = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[i].m_n[2]->m_x);//.rescale(2.0);				
//			
//			ofxVec3f norm = ofxBulletStaticUtil::btVec3ToOfxVec3(faces[i].m_normal);
//			norm *= 50;
//			
//			ns.push_back(ofxBulletStaticUtil::ofxVec3ToBtVec3(a+norm));
//			ns.push_back(ofxBulletStaticUtil::ofxVec3ToBtVec3(b+norm));
//			ns.push_back(ofxBulletStaticUtil::ofxVec3ToBtVec3(c+norm));			
//				
//	
//		}
//	}
//	yesORno = bullet->createSoftConvexHull(ofxVec3f(0, 80, 0), &ns[0], ns.size());	
	
}

void YesNoObjectSoft::pinchFace(int faceIdx) {
	// get face and nodes
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	btSoftBody::Node* node_0 = faces[faceIdx].m_n[0];
	btSoftBody::Node* node_1 = faces[faceIdx].m_n[1];
	btSoftBody::Node* node_2 = faces[faceIdx].m_n[2];
	
	// pinch face
	int pinchFaceFactor = ofMap(resolusion, minRes, maxRes, 67000, 9700);
	btVector3 normal = faces[faceIdx].m_normal;
	btVector3 pinchFaceForce = normal*pinchFaceFactor;
	
	if (node_0->m_im > 0) node_0->m_f += pinchFaceForce;
	if (node_1->m_im > 0) node_1->m_f += pinchFaceForce;
	if (node_2->m_im > 0) node_2->m_f += pinchFaceForce;	

}

void YesNoObjectSoft::blowUp() {
	// get face and nodes
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	int fSize = faces.size();
	
	int pinchFaceFactor = ofMap(resolusion, minRes, maxRes, 37000, 2700);
	
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

void YesNoObjectSoft::finish() {
	shrinkTimer.setParameters(easing, ofxTween::easeIn, 0, 100, 2000, 0);
	bFinish = true;
}

