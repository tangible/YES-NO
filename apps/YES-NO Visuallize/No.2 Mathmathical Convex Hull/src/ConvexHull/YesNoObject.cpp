/*
 *  YesNoObject.cpp
 *  No.2 Mathmathical Convex Hull
 *
 *  Created by Makira on 11/02/10.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "YesNoObject.h"

void YesNoObject::setup(ofxBullet* _bullet, ofxVec3f _forcePoint, int initialNumObj) {

	bullet = _bullet;
	forcePoint = _forcePoint;
	maxValu = 0.01;
	
	addSMS(initialNumObj);
	
}

void YesNoObject::update() {

	for (int j = 0; j < smss.size(); j++) {
		MyRigidBody* sms = smss[j]->sms;
		ofxVec3f force = ofxVec3f(-sms->getBodyPos() + forcePoint);
		force *= maxValu * 6;
		btVector3 impulse(force.x, force.y, force.z);
		sms->getRigidBody()->applyCentralImpulse(impulse);
		smss[j]->update();
	}
	
}

int prevFaceNum = 0;
int faceIndex = 0;
void YesNoObject::draw(ofxVec4f baseCol) {
	
	float vertices[smss.size()*3];
	
	for (int i = 0; i < smss.size(); i++) {
		MyRigidBody* sms = smss[i]->sms;
		ofxVec3f pos = sms->getBodyPos();
		vertices[3*i] = pos.x;
		vertices[3*i+1] = pos.y;
		vertices[3*i+2] = pos.z;
	}
	if (smss.size() > 3) {
		ch3d = new Chull3D(vertices, (int)smss.size());
		ch3d->compute();
		float* points;
		int nPoints = 0;
		int* faces;
		int nFaces = 0;	
		ch3d->get_convex_hull(&points, &nPoints, &faces, &nFaces);
		
		faceIndex++;
		if (prevFaceNum != nFaces) {
			string fn = "/Users/makira/Project/20110321.VancouverMusium/tmp/"+ofToString(faceIndex)+".txt";
			ch3d->export_obj((char*)fn.c_str());
		}
		prevFaceNum = nFaces;
						 
		for (int i = 0; i < nFaces; i++) {
			vector<int> fp;
			fp.push_back(faces[3*i]);
			fp.push_back(faces[3*i+1]);
			fp.push_back(faces[3*i+2]);		
			
			// draw edge
			ofSetLineWidth(3);
			glColor4f(0.0, 0.0, 0.0, 10);
			glBegin(GL_LINE_LOOP);			
			for (int j = 0; j < fp.size(); j++) {
				int idx = fp[j];
				glVertex3f(points[3*idx], points[3*idx+1], points[3*idx+2]);
			}
			glEnd();
			
			// calc col
			float cptr[12];
			calcFaceColor(points, fp, &cptr[0]);
			
			// draw face
			float test[10];	
			float vptr[9];
			for (int j = 0; j < fp.size(); j++) {
				int idx = fp[j];
				vptr[3*j] = points[3*idx];
				vptr[3*j+1] = points[3*idx+1];
				vptr[3*j+2] = points[3*idx+2];
			}
			glEnable( GL_SMOOTH );
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);			
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 0, cptr);
			glVertexPointer(3, GL_FLOAT, 0, vptr);
			glNormalPointer(GL_FLOAT, 0, vptr);
			glTexCoordPointer(3, GL_FLOAT, 0, vptr);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);			
			glDisableClientState(GL_COLOR_ARRAY);			
			
		}
		
		
		delete ch3d;
		delete [] points;
		delete [] faces;
	}	
	
}

// 新しく入ってきたSMSを基準として、全体の色を再構成する
void YesNoObject::calcFaceColor(float* points, vector<int> faceDef, float* colPtrRtn) {
	
	// scale - age dependent
	// angle - position dependent
	
	Vertex *tgtVertex;
	float cur = (float)ofGetElapsedTimeMillis();
	float tmpAge = 0.0;
	float tmpAngle = 0.0;
	for (int i = 0; i < faceDef.size(); i++) { // compute col by point's age
		int idx = faceDef[i];
		ofxVec3f facePos = ofxVec3f(points[3*idx], points[3*idx+1], points[3*idx+2]);
		for (int j = 0; ; j++) {					
			ofxVec3f bodyPos = smss[j]->sms->getBodyPos();
			if (facePos == bodyPos) {
				tgtVertex = smss[j];
				break;
			}
		}
		float brn = (float)tgtVertex->sms->age;
		tmpAge += cur-brn;
		tmpAngle += tgtVertex->col.getColorAngle();
	}
	
	float avrgAge = tmpAge / faceDef.size();
	float avrgAng = tmpAngle / faceDef.size();
	float scale = ofMap(avrgAge, 0.0, cur, 1.0, -1.0);
	float angle = ofMap(avrgAge, 0.0, cur, 0.0, 1.0);
	
	for (int i = 0; i < faceDef.size(); i++) {
		//tgtVertex->col.setColorScale(scale);
		//tgtVertex->col.setColorAngle(angle);
		ofColor tgtCol = tgtVertex->col.getColor();
		
		colPtrRtn[4*i] = tgtCol.r /255.0;
		colPtrRtn[4*i+1] = tgtCol.g /255.0;
		colPtrRtn[4*i+2] = tgtCol.b /255.0;
		colPtrRtn[4*i+3] = tgtCol.a /255.0;	
	}
	
//	for (int j = 0; j < faceDef.size(); j++) { // compute col by point's age
//		int idx = faceDef[j];
//		ofxVec3f facePos = ofxVec3f(points[3*idx], points[3*idx+1], points[3*idx+2]);
//		Vertex* tgtVertex;
//		for (int k = 0; ; k++) {					
//			ofxVec3f bodyPos = smss[k]->sms->getBodyPos();
//			if (facePos == bodyPos) {
//				tgtVertex = smss[k];
//				break;
//			}
//		}
//		float cur = (float)ofGetElapsedTimeMillis();
//		float brn = (float)tgtVertex->sms->age;
//		float age = cur-brn;
//		float scale = ofMap(age, 0.0, cur, 1.0, -1.0);
//		
//		tgtVertex->col.setColorScale(scale);
//		tgtVertex->col.setColorAngle(tgtVertex->col.getColorAngle()+0.001);
//		ofColor tgtCol = tgtVertex->col.getColor();
//		
//		colPtrRtn[4*j] = tgtCol.r /255.0;
//		colPtrRtn[4*j+1] = tgtCol.g /255.0;
//		colPtrRtn[4*j+2] = tgtCol.b /255.0;
//		colPtrRtn[4*j+3] = tgtCol.a /255.0;				
//		
//	}	
	
}

void YesNoObject::debugDraw() {
	
	for (int i = 0; i < smss.size(); i++) {
		smss[i]->sms->render(bullet->getWorld());
	}
	
}

float smsIdx = 0.0;
void YesNoObject::addSMS(int numSMS, int YesOrNo, ofxVec3f pos) {

	for (int i = 0; i < numSMS; i++) {
		
		ofxVec3f rdmPos;
		if (pos == 0) {
			rdmPos = ofxVec3f(ofRandom(forcePoint.x-250, forcePoint.x+250),
							  -200,
							  ofRandom(forcePoint.z-250, forcePoint.z+250));			
		}else {
			rdmPos = ofxVec3f(ofRandom(pos.x-250, pos.x+250),
							  ofRandom(pos.y-250, pos.y+250),
							  ofRandom(pos.z-250, pos.z+250));
		}

		MyRigidBody* obj = bullet->createBox(rdmPos, 
											 ofxVec3f(ofRandom(30, 70), ofRandom(30, 70), ofRandom(30, 70)),
											 1, ofxVec4f(ofRandom(0.0, 1.0), ofRandom(0.0, 1.0), ofRandom(0.0, 1.0), ofRandom(0.3, 0.3)), 
											 DYNAMIC_BODY);
		Vertex* v = new Vertex();
		v->col.setColorRadius(1.0);
		if (YesOrNo == 0) {
			v->angleMin = 0.0;
			v->angleMax = 0.5;
		}else if (YesOrNo == 1) {
			v->angleMin = 0.5;
			v->angleMax = 1.0;			
		}
		
		if (smsIdx >= 1.0) {
			smsIdx = 0.0;
		}else {
			smsIdx += 0.01;
		}
		v->col.setColorAngle(smsIdx);
		v->sms = obj;
		smss.push_back(v);
	}

}
