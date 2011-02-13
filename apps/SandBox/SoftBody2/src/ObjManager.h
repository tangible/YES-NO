/*
 *  objManager.h
 *  akiraOF23ofxBulletBasics
 *
 *  Created by 下園 麻貴 on 10/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"
#include <btBulletDynamicsCommon.h>
#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "MySoftBody.h"

class ObjManager {
	
	protected:
	
		btDynamicsWorld* m_dynamicsWorld;
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher*	m_dispatcher;
		btBroadphaseInterface*	m_broadphase;
		btConstraintSolver*	m_solver;
		
		btSoftRigidDynamicsWorld* m_soft_dynamicsWorld;
		btSoftBodyWorldInfo softBodyWI;
		
		btCollisionShape* groundShapeContainer;
		vector<MySoftBody*> softBodyContainers;

		ofxVec3f sceneCenter;
		
		
	
	public:
	
		ObjManager();
		ObjManager(bool soft);
		~ObjManager();
		
		void createSphere();
		void update();
		void render();
		
		btSoftBody* psb;
		void createSoftBody();
		void updateSoftBody();
		void renderSoftBody();
		
		ofxVec3f getSceneCenter();
		void setSceneCenter(ofxVec3f _center);
		int getNumSpheres();		
};
