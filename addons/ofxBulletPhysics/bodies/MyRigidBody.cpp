
#include "MyRigidBody.h"

void MyRigidBody::remove(btDynamicsWorld* m_dynamicsWorld) {
	if (psb && psb->getMotionState()) {
		delete psb->getMotionState();
	}	
	m_dynamicsWorld->removeCollisionObject(psb);
	delete psb;
	delete shape;	
}

void MyRigidBody::createBoxShape(btTransform startTrans, btVector3 boxShape, int mass, ofxVec4f color) {
	boxSize = boxShape;
	bodyColor = color;
	shape = new btBoxShape(boxShape);
	createRigidBody(mass, startTrans);
}

void MyRigidBody::createSphereShape(btTransform startTrans, int _radius, int mass, ofxVec4f color) {
	sphereRadius = _radius;
	bodyColor = color;
	shape = new btSphereShape(sphereRadius);
	createRigidBody(mass, startTrans);	
}

void MyRigidBody::createCapsuleShape(btTransform startTrans, int radius, int height, int mass, ofxVec4f color) {
	bodyColor = color;
	shape = new btCapsuleShape(radius, height);
	createRigidBody(mass, startTrans);	
}


void MyRigidBody::createRigidBody(int mass, const btTransform startTrans) {

	btVector3 inertia(0,0,0);
	shape->calculateLocalInertia(mass,inertia);
	
	btMotionState* myMotionState;
	switch (bodyType) {
		case 0:
			myMotionState = new btDefaultMotionState(startTrans);
			mass = 0;
			break;
		case 1:
			myMotionState = new btDefaultMotionState(startTrans);
			break;
		case 2:
			myMotionState = new MyKinematicMotionState(startTrans);
			break;
			
		default:
			myMotionState = new btDefaultMotionState(startTrans);			
			break;
	}	
	
	btRigidBody::btRigidBodyConstructionInfo rbci(mass,myMotionState,shape,inertia);
	rbci.m_startWorldTransform = startTrans;	

	psb = new btRigidBody(rbci);
	
	if (bodyType == 2) {
		psb->setCollisionFlags(psb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		psb->setActivationState(DISABLE_DEACTIVATION);
	}
}


void MyRigidBody::translateBody(ofxVec3f pos, ofxVec3f rotDir, float degree) {
	
//	cout << "only for kinematic body...?" << endl;
//	assert(bodyType == KINEMATIC_BODY);
	
	btTransform currentPos;
	currentPos.setIdentity();	
	currentPos.setOrigin(ofxBulletStaticUtil::ofxVec3ToBtVec3(pos));
	
	if (degree != 0) {
		btQuaternion q;
		q.setRotation(ofxBulletStaticUtil::ofxVec3ToBtVec3(rotDir), ofxBulletStaticUtil::degToRad(degree));
		currentPos.setRotation(q);
	}
	
	MyKinematicMotionState* myMotionState = (MyKinematicMotionState*)psb->getMotionState();
	myMotionState->setKinematicPos(currentPos);
	
}


void MyRigidBody::render(btDynamicsWorld* m_dynamicsWorld) {

	btPolyhedralConvexShape* shape = (btPolyhedralConvexShape*)psb->getCollisionShape();
	
	btScalar	m[16];
	psb->getWorldTransform().getOpenGLMatrix(m);
	
	btVector3 aabbMin,aabbMax;
	m_dynamicsWorld->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
	
	drawer.drawOpenGL(m, shape, btVector3(1, 0, 0), 0, aabbMin, aabbMax, ofxBulletStaticUtil::ofxVec4ToBtVec4(bodyColor));
	
	ofSetColor(255, 255, 255);
}

vector<ofxVec3f> MyRigidBody::getVertsPos() {
	
	vector<ofxVec3f> ret;
	
	GL_ShapeDrawer::ShapeCache *sc = drawer.cache((btConvexShape*)shape);
	btShapeHull* hull = &sc->m_shapehull;
	
	if (hull->numTriangles() > 0) {
		
		int index = 0;
		const unsigned int *idx = hull->getIndexPointer();
		const btVector3 *vtx = hull->getVertexPointer();		
		
		for (int i = 0; i < hull->numTriangles(); i++) {
		
			int i1 = index++;
			int i2 = index++;
			int i3 = index++;
			assert(i1 < hull->numIndices () &&
					i2 < hull->numIndices () &&
					i3 < hull->numIndices ());
			
			int index1 = idx[i1];
			int index2 = idx[i2];
			int index3 = idx[i3];
			assert(index1 < hull->numVertices () &&
					index2 < hull->numVertices () &&
					index3 < hull->numVertices ());
			
			// triangle
			btVector3 b1 = vtx[index1];
			btVector3 b2 = vtx[index2];
			btVector3 b3 = vtx[index3];			
			//btVector3 bc = (b1+b2+b3)/3;
			
			ret.push_back(ofxBulletStaticUtil::btVec3ToOfxVec3(b1));
			ret.push_back(ofxBulletStaticUtil::btVec3ToOfxVec3(b2));
			ret.push_back(ofxBulletStaticUtil::btVec3ToOfxVec3(b3));
			//ret.push_back(ofxBulletStaticUtil::btVec3ToOfxVec3(bc));
			
		}
	}
	
	return ret;
}

ofPoint MyRigidBody::getBodyPos() {

	btScalar m[16];
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)psb->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
	btVector3 org(m[12], m[13], m[14]);
	return ofPoint(org.x(), org.y(), org.z());
	
//	btTransform trans;
//	btDefaultMotionState* myMotionState = (btDefaultMotionState*)psb->getMotionState();
//	myMotionState->getWorldTransform(trans);
//	btVector3 btPos = trans.getOrigin();
//	return ofPoint(btPos.x(), btPos.y(), btPos.z());	
}

ofPoint MyRigidBody::getBodyRotDegree() {
	
	btScalar m[16];
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)psb->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);	
	float fAngZ = atan2f(m[1], m[5]);
	float fAngY = atan2f(m[8], m[10]);
	float fAngX = -asinf(m[9]);	
	return ofPoint(ofxBulletStaticUtil::radToDeg(fAngX), 
				   ofxBulletStaticUtil::radToDeg(fAngY), 
				   ofxBulletStaticUtil::radToDeg(fAngZ));
}

ofxVec4f MyRigidBody::getBodyColor() {
	return bodyColor;
}

int MyRigidBody::getSphereRadius() {
	return sphereRadius;
}

ofxVec3f MyRigidBody::getBoxSize() {
	return ofxBulletStaticUtil::btVec3ToOfxVec3(boxSize);
}