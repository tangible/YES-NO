/*
 *  objManager.cpp
 *  akiraOF23ofxBulletBasics
 *
 *  Created by 下園 麻貴 on 10/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "ObjManager.h"

#define G_POS 0
#define G_WIDTH 7000

//setup Ground and SoftBody
ObjManager::ObjManager(bool soft) {

	sceneCenter.set(ofGetWidth()/2, ofGetHeight()/2, 0);

    btVector3 worldAabbMin(-1000,-1000,-1000);
    btVector3 worldAabbMax(1000,1000,1000);
    int maxProxies = 1024;
    m_broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
    softBodyWI.m_broadphase = m_broadphase;

    btDefaultCollisionConfiguration* collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(collisionConfiguration);
    softBodyWI.m_dispatcher = m_dispatcher;

    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    m_soft_dynamicsWorld = new btSoftRigidDynamicsWorld(m_dispatcher,m_broadphase,solver,collisionConfiguration);
    m_soft_dynamicsWorld->setGravity(btVector3(0,-10,0));
    softBodyWI.m_gravity.setValue(0,10,0);
	
	softBodyWI.air_density = 10;
	softBodyWI.water_density = 10;
	softBodyWI.m_sparsesdf.Initialize();

    psb = btSoftBodyHelpers::CreateEllipsoid(softBodyWI,btVector3(0,0,0),
                                                        btVector3(3,6,8),1024);
	psb->m_materials[0]->m_kLST   =   0.1;

	// 粘り気
	psb->m_cfg.kDP            =   0.001;//0.001; 
	// 地面との間で巻き込まれたのが回復する力
	psb->m_cfg.kDG			  =   0;
	// わからない。何かへの抵抗。
	psb->m_cfg.kLF			  =   0;
	// ???
	psb->m_cfg.kDF            =   1;//1;
	// 元に戻る力、弾力
	psb->m_cfg.kPR            =   2500;//2500;
	// ぐちゃぐちゃになる度。中身の量。元に戻らない度。
	psb->m_cfg.kVC            =   0;//2500;
	// 地面と滑るかどうか
	psb->m_cfg.kDF            =   1;//1;
	// ???
	psb->m_cfg.kMT            =   0;//1;	
	// ???
	psb->m_cfg.kCHR            =   1;//1;	
	// ???
	psb->m_cfg.kSHR            =   1;//1;
	
	psb->setTotalMass(20,true);
	psb->setPose( false, false );

	m_soft_dynamicsWorld->addSoftBody(psb);
	psb->translate(btVector3(0,-50,0));


//	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,G_POS,0),1);
//	btTransform initTransform;
//	initTransform.setIdentity();
//	initTransform.setOrigin(btVector3(0,G_POS,0));
//	btDefaultMotionState* groundMotionState = new btDefaultMotionState(initTransform);
//	
//	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
//	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
//	m_soft_dynamicsWorld->addRigidBody(groundRigidBody);		


    btCollisionShape* Shape = new btBoxShape(btVector3(50,5,50));
	btTransform initTransform;
	initTransform.setIdentity();
	initTransform.setOrigin(btVector3(0,G_POS,0));
    btMotionState* motionState = new btDefaultMotionState(initTransform);

    btRigidBody::btRigidBodyConstructionInfo BodyCI(0,motionState,Shape,btVector3(0,0,0));
    btRigidBody* rBody = new btRigidBody(BodyCI);
    m_soft_dynamicsWorld->addRigidBody(rBody);
	

}

// step the world
void ObjManager::updateSoftBody() {
	m_soft_dynamicsWorld->stepSimulation(1.0f/60.0f, 10);
}

// render
void ObjManager::renderSoftBody() {
	
//	cout << psb->m_nodes[0].m_x[1] << endl;

	ofSetRectMode(OF_RECTMODE_CENTER);
	
	glColor4f(0.9f, .0f, .0f, 0.5f);
	
	
	glPushMatrix();
		glTranslatef(sceneCenter.x, sceneCenter.y, sceneCenter.z);
		
		btSoftBody::tNodeArray&   nodes(psb->m_nodes);
		btSoftBody::tLinkArray&   links(psb->m_links);
		btSoftBody::tFaceArray&   faces(psb->m_faces);

		for(int j=0;j<nodes.size();++j) {
			ofxPoint(nodes[j].m_x.getX(), nodes[j].m_x.getY(), nodes[j].m_x.getZ());
		}

		for(int j=0;j<links.size();++j) {
			btSoftBody::Node*   node_0=links[j].m_n[0];
			btSoftBody::Node*   node_1=links[j].m_n[1];
			ofxLine(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(), 
				  node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ());

		}

		for(int j=0;j<faces.size();++j) {
			btSoftBody::Node*   node_0=faces[j].m_n[0];
			btSoftBody::Node*   node_1=faces[j].m_n[1];
			btSoftBody::Node*   node_2=faces[j].m_n[2];
			ofxTriangleShape(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(),
					  node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ(),
					  node_2->m_x.getX(), node_2->m_x.getY(), node_2->m_x.getZ());


		}
		
		ofxVec3f p1(-G_WIDTH, G_POS, -G_WIDTH);
		ofxVec3f p2(G_WIDTH, G_POS, -G_WIDTH);
		ofxVec3f p3(G_WIDTH, G_POS, G_WIDTH);
		ofxVec3f p4(-G_WIDTH, G_POS, G_WIDTH);
		glColor4f(.0f, .0f, .5f, 0.08f);
		ofFill();
		ofxQuad(p1, p2, p3, p4);		
	glPopMatrix();
}



