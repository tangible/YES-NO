/*
 *  YesNoObjectSoft.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/12.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "YesNoObjectSoft.h"

void YesNoObjectSoft::setup(ofxBullet* _bullet, ofxVec3f _forcePoint, int res, ofxVec3f scale) {

	bullet = _bullet;
	forcePoint = _forcePoint;
	maxValu = 0.01;
	
	ofxVec3f gravity(0, 80, 0);
	ofxVec3f center(forcePoint);
	radius = scale;
	radius *= 4;
	resolusion = res;
	yesORno = bullet->createEllipsoid(gravity, center, radius, resolusion);	
	
	col.setColorRadius(1.0);
	col.setColorScale(0.25);
	col.setColorAngle(0.1);
	
	changeColBySMSRecievedFace(yesORno->getSoftBody()->m_faces, 0);
}

int pfidx = 0;
void YesNoObjectSoft::update() {
	
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
	glColorPointer(4, GL_FLOAT, 0, &colorPointer[0]);	
	glDrawArrays(GL_TRIANGLES, 0, faceVertexPtr.size()/3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	
	glDisableClientState(GL_COLOR_ARRAY);	
	
//	ofSetColor(0, 0, 0);
//	for(int i = 0; i < faces.size(); i++) {
//		btSoftBody::Node* node_0 = faces[i].m_n[0];
//		btSoftBody::Node* node_1 = faces[i].m_n[1];
//		btSoftBody::Node* node_2 = faces[i].m_n[2];
//		ofxTriangleShape(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(),
//						 node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ(),
//						 node_2->m_x.getX(), node_2->m_x.getY(), node_2->m_x.getZ());	
//		
//	}		

}

void YesNoObjectSoft::debugDraw() {

	yesORno->render();

}

/*
 scale - age dependent
 angle - position dependent 
 0                     9                     18
 faceVertexPtr = (x,y,z, x,y,z, x,y,z,  x,y,z, x,y,z, x,y,z,  x,y,z, x,y,z, x,y,z....)
 */
vector<float> YesNoObjectSoft::caclColAngleAndScaleForObj(btSoftBody::tFaceArray& faces) {
	
	vector<float> colPtrRtn;
	
	btSoftBody::Node* node_0 = faces[0].m_n[0];
	btSoftBody::Node* node_1 = faces[0].m_n[1];
	btSoftBody::Node* node_2 = faces[0].m_n[2];
	ofxVec3f nodeCentroid = ofxVec3f((node_0->m_x.getX()+node_1->m_x.getX()+node_2->m_x.getX())/3,
									 (node_0->m_x.getY()+node_1->m_x.getY()+node_2->m_x.getY())/3,
									 (node_0->m_x.getZ()+node_1->m_x.getZ()+node_2->m_x.getZ())/3);
	
	float mostFar = 0.0;
	for (int faceIdx = 0; faceIdx < faces.size(); faceIdx++) {
		
		btSoftBody::Node* compnode_0 = faces[faceIdx].m_n[0];
		btSoftBody::Node* compnode_1 = faces[faceIdx].m_n[1];
		btSoftBody::Node* compnode_2 = faces[faceIdx].m_n[2];
		ofxVec3f compNodeCentroid = ofxVec3f((compnode_0->m_x.getX()+compnode_1->m_x.getX()+compnode_2->m_x.getX())/3,
											 (compnode_0->m_x.getY()+compnode_1->m_x.getY()+compnode_2->m_x.getY())/3,
											 (compnode_0->m_x.getZ()+compnode_1->m_x.getZ()+compnode_2->m_x.getZ())/3);
		
		float distance = nodeCentroid.distance(compNodeCentroid);  // ***
		if (distance > mostFar) mostFar = distance;
	}
	
	for (int faceIdx = 0; faceIdx < faces.size(); faceIdx++) {
		
		btSoftBody::Node* compnode_0 = faces[faceIdx].m_n[0];
		btSoftBody::Node* compnode_1 = faces[faceIdx].m_n[1];
		btSoftBody::Node* compnode_2 = faces[faceIdx].m_n[2];
		ofxVec3f compNodeCentroid = ofxVec3f((compnode_0->m_x.getX()+compnode_1->m_x.getX()+compnode_2->m_x.getX())/3,
											 (compnode_0->m_x.getY()+compnode_1->m_x.getY()+compnode_2->m_x.getY())/3,
											 (compnode_0->m_x.getZ()+compnode_1->m_x.getZ()+compnode_2->m_x.getZ())/3);
		
		float distance = nodeCentroid.distance(compNodeCentroid);  // ***
		float angle = ofMap(distance, 0.0, mostFar, 0.0, 0.5);
//		cout << ofToString(angle) << endl;
		
		for (int i = 0; i < 3; i++) {
			col.setColorAngle(angle);
			col.update();
			ofColor c = col.getColor();
			colPtrRtn.push_back(c.r/255.0);
			colPtrRtn.push_back(c.g/255.0);
			colPtrRtn.push_back(c.b/255.0);
			colPtrRtn.push_back(1.0);			
		}
		
	}	
	
	colorPointer = colPtrRtn;
	return colPtrRtn;
}

vector<float> YesNoObjectSoft::changeColBySMSRecievedFace(btSoftBody::tFaceArray& faces, int faceID) {

	vector<int> faceIDVec = sortFaceByDistance(faces, faceID);
	
	vector<float> colPtrRtn;
	int test = faces.size()*3*4;
	for (int i = 0; i < faces.size()*3*4; i++) {
		colPtrRtn.push_back(0.0);
	}
	float * colPtr = &colPtrRtn[0];
	
	float angle = 0.0;
	float angleStep = 0.005;//1.0/faces.size();
	for (int i = 0; i < faces.size(); i++) {
		btSoftBody::Node* compnode_0 = faces[faceIDVec[i]].m_n[0];
		btSoftBody::Node* compnode_1 = faces[faceIDVec[i]].m_n[1];
		btSoftBody::Node* compnode_2 = faces[faceIDVec[i]].m_n[2];		
		
		for (int j = 0; j < 3; j++) {
			
			int idx = faceIDVec[i]*12+j*4;
			col.setColorAngle(angle);
			col.update();
			ofColor c = col.getColor();
			colPtr[idx] = c.r/255.0;
			colPtr[idx+1] = c.g/255.0;
			colPtr[idx+2] = c.b/255.0;
			colPtr[idx+3] = 1.0;					
		}		
		
		angle += angleStep;
	}

	colorPointer = colPtrRtn;
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
	
//	vector<int> faceIDVec;
//	for (int i = 0; i < faces.size(); i++) {
//		faceIDVec.push_back(i);
//	}
//	
//	int i;
//	int j;
//	
//	ofxVec3f tgtCentroid = getFaceCentroid(faces, faceID);
//	for (i = 0; i < faceIDVec.size()-1; i++) {
//		for (j = i+1; j < faceIDVec.size(); j++) {
//			ofxVec3f compCentroid1 = getFaceCentroid(faces, i);
//			ofxVec3f compCentroid2 = getFaceCentroid(faces, j);
//			float d1 = tgtCentroid.distance(compCentroid1);
//			float d2 = tgtCentroid.distance(compCentroid2);
//			if (d1 > d2) {
//				faceIDVec[i] = j;
//				faceIDVec[j] = i;				
////				int tmp = faceIDVec[i];
////				faceIDVec[i] = faceIDVec[j];
////				faceIDVec[j] = tmp;
//			}
//		}
//	}
	
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
		
//		btSoftBody::tNodeArray& nodes(yesORno->getSoftBody()->m_nodes);	
//		int rdmIdx = ofRandom(0, nodes.size()-1);
//		ofxVec3f rdmVert = ofxBulletStaticUtil::btVec3ToOfxVec3(nodes[rdmIdx].m_x);
//		ofxVec3f rdmNorm = ofxBulletStaticUtil::btVec3ToOfxVec3(nodes[rdmIdx].m_n);
//		ofxVec3f newVert = rdmVert*(rdmNorm);
//
//		yesORno->getSoftBody()->appendNode(ofxBulletStaticUtil::ofxVec3ToBtVec3(newVert), 1);
//		
//		cout << "newVert.x="+ofToString(newVert.x)+" y="+ofToString(newVert.y)+" z="+ofToString(newVert.z) << endl;
//		
//		vector<btVector3> v;
//		for (int i = 0; i < nodes.size(); i++) {
//			v.push_back(nodes[i].m_x);
//		}
//		
//		HullDesc		hdsc(QF_TRIANGLES,nodes.size(),&v[0]);
//		HullResult		hres;
//		HullLibrary		hlib;/*??*/ 
//		hdsc.mMaxVertices=nodes.size();
//		hlib.CreateConvexHull(hdsc,hres);
//		for(int i=0;i<(int)hres.mNumFaces;++i)
//		{
//			const int idx[]={	hres.m_Indices[i*3+0],
//				hres.m_Indices[i*3+1],
//				hres.m_Indices[i*3+2]};
//			if(idx[0]<idx[1]) yesORno->getSoftBody()->appendLink(	idx[0],idx[1]);
//			if(idx[1]<idx[2]) yesORno->getSoftBody()->appendLink(	idx[1],idx[2]);
//			if(idx[2]<idx[0]) yesORno->getSoftBody()->appendLink(	idx[2],idx[0]);
//			yesORno->getSoftBody()->appendFace(idx[0],idx[1],idx[2]);
//		}
//		hlib.ReleaseResult(hres);
//		yesORno->getSoftBody()->randomizeConstraints();			
		
	}
	if (key == 'g') {
		yesORno->softBodyWI.m_gravity.setValue(0, 10, 0);
	}
}

void YesNoObjectSoft::addSMS(int faceID) {
	
	pinchFace(faceID);
	colorPointer.clear();
	changeColBySMSRecievedFace(yesORno->getSoftBody()->m_faces, faceID);
	
}

void YesNoObjectSoft::pinchFace(int faceIdx) {
	// get face and nodes
	btSoftBody::tFaceArray& faces(yesORno->getSoftBody()->m_faces);
	btSoftBody::Node* node_0 = faces[faceIdx].m_n[0];
	btSoftBody::Node* node_1 = faces[faceIdx].m_n[1];
	btSoftBody::Node* node_2 = faces[faceIdx].m_n[2];
	
	// pinch face
	int pinchFaceFactor = ofMap(resolusion, minRes, maxRes, 27000, 2700);
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
	int pinchFaceFactor = ofMap(resolusion, minRes, maxRes, 1000, 100);
	
	for (int i = 0; i < fSize; i++) {
		pinchFace(i);	
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