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
	
	//insms.setup(forcePoint);
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


void YesNoObject::draw() {
	
	//insms.debugDraw();
	
	ofxVec3f pos = smss[smss.size()-1]->sms->getBodyPos();
	ofxSphere(pos, 20);

	vector<float> vertices;
	for (int i = 0; i < smss.size(); i++) {
		MyRigidBody* sms = smss[i]->sms;
		ofxVec3f pos = sms->getBodyPos();
		vertices.push_back(pos.x);
		vertices.push_back(pos.y);
		vertices.push_back(pos.z);
	}	
	if (smss.size() > 3) {
		ch3d = new Chull3D(&vertices[0], (int)smss.size());
		ch3d->compute();
		float* points;
		int nPoints = 0;
		int* faces;
		int nFaces = 0;	
		ch3d->get_convex_hull(&points, &nPoints, &faces, &nFaces);

		vector<float> faceVertexPtr;
		for (int i = 0; i < nFaces; i++) {
			vector<int> fp;
			fp.push_back(faces[3*i]);
			fp.push_back(faces[3*i+1]);
			fp.push_back(faces[3*i+2]);		
			
			// draw edge
			ofSetLineWidth(3);
			glColor4f(0.0, 0.0, 0.0, 1.0);
			glBegin(GL_LINE_LOOP);			
			for (int j = 0; j < fp.size(); j++) {
				int idx = fp[j];
				glVertex3f(points[3*idx], points[3*idx+1], points[3*idx+2]);
			}
			glEnd();
			
			for (int j = 0; j < fp.size(); j++) {
				int idx = fp[j];
				faceVertexPtr.push_back(points[3*idx]);
				faceVertexPtr.push_back(points[3*idx+1]);
				faceVertexPtr.push_back(points[3*idx+2]);							   
			}						
		}		

		
		vector<float> colPtr = caclColAngleAndScaleForObj(faceVertexPtr, nFaces);
		
		glEnable( GL_SMOOTH );
		glEnableClientState(GL_VERTEX_ARRAY);		
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &faceVertexPtr[0]);
		glNormalPointer(GL_FLOAT, 0, &faceVertexPtr[0]);
		glTexCoordPointer(3, GL_FLOAT, 0, &faceVertexPtr[0]);	
		glColorPointer(4, GL_FLOAT, 0, &colPtr[0]);
		glDrawArrays(GL_TRIANGLES, 0, faceVertexPtr.size()/3);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);			
		glDisableClientState(GL_COLOR_ARRAY);
		
		delete ch3d;
		delete [] points;
		delete [] faces;
	}	
	
}

/*
 scale - age dependent
 angle - position dependent 
                  0                     9                     18
 faceVertexPtr = (x,y,z, x,y,z, x,y,z,  x,y,z, x,y,z, x,y,z,  x,y,z, x,y,z, x,y,z....)
 */
vector<float> YesNoObject::caclColAngleAndScaleForObj(vector<float> faceVertexPtr, int nFaces) {
	
	vector<float> colPtrRtn;
	
	// calc angle
	Vertex* incomingSMS = smss[smss.size()-1];
	ofxVec3f inSMSPos = incomingSMS->sms->getBodyPos();

	for (int faceIdx = 0; faceIdx < nFaces; faceIdx++) {
		
		vector<Vertex*> facet; // facet has 3 vertice
		for (int vertIdx = 0; vertIdx < 3; vertIdx++) { // each vertex in facet	
			vector<float> coords;
			for (int coordIdx = 0; coordIdx < 3; coordIdx++) {
				int cIdx = (9*faceIdx+(3*vertIdx))+coordIdx;				
				float coord = faceVertexPtr[cIdx];
				coords.push_back(coord);
			}
			ofxVec3f pos = ofxVec3f(coords[0], coords[1], coords[2]);
			Vertex* vert = getVertexByPos(pos);
			facet.push_back(vert);
		}
		
		float x = (facet[0]->sms->getBodyPos().x+facet[1]->sms->getBodyPos().x+facet[2]->sms->getBodyPos().x)/3;
		float y = (facet[0]->sms->getBodyPos().y+facet[1]->sms->getBodyPos().y+facet[2]->sms->getBodyPos().y)/3;
		float z = (facet[0]->sms->getBodyPos().z+facet[1]->sms->getBodyPos().z+facet[2]->sms->getBodyPos().z)/3;
		ofxVec3f centroid = ofxVec3f(x, y, z);
		float distance = inSMSPos.distance(centroid)/800;  // ***
		
		for (int vertIdx = 0; vertIdx < 3; vertIdx++) {
			facet[vertIdx]->col.setColorAngle(distance);
			//facet[vertIdx]->col.setColorScale(1.0-distance);
			facet[vertIdx]->col.update();
			ofColor c = facet[vertIdx]->col.getColor();
			colPtrRtn = setColPtr(c, colPtrRtn);
		}	
	}	

	return colPtrRtn;
}

vector<float> YesNoObject::setColPtr(ofColor tgtCol, vector<float> colPtrRtn) {

	colPtrRtn.push_back(tgtCol.r /255.0);
	colPtrRtn.push_back(tgtCol.g /255.0);
	colPtrRtn.push_back(tgtCol.b /255.0);
	colPtrRtn.push_back(1.0);	
	return colPtrRtn;		
	
}

Vertex* YesNoObject::getVertexByPos(ofxVec3f pos) {
	
	for (int i = 0; i < smss.size(); i++) {
		ofxVec3f vertPos = smss[i]->sms->getBodyPos();
		if (vertPos == pos) {
			return smss[i];
		}
	}
	return NULL; // shoud not reach here...
	
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

		IncomingSMS sms;
		//sms.setup(rdmPos);
		//insmss.push_back(sms);
		
		
		MyRigidBody* obj = bullet->createBox(rdmPos, 
											 ofxVec3f(ofRandom(30, 70), ofRandom(30, 70), ofRandom(30, 70)),
											 1, ofxVec4f(ofRandom(0.0, 1.0), ofRandom(0.0, 1.0), ofRandom(0.0, 1.0), ofRandom(0.3, 0.3)), 
											 DYNAMIC_BODY);
		//obj->getRigidBody()->setAngularFactor(0); // like particle
		
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
