#include "MySoftBody.h"

/*
 
 btScalar				m_kLST;			// Linear stiffness coefficient [0,1]
 btScalar				m_kAST;			// Area/Angular stiffness coefficient [0,1]
 btScalar				m_kVST;			// Volume stiffness coefficient [0,1]
 int						m_flags;		// Flags
 
 eAeroModel::_			aeromodel;		// Aerodynamic model (default: V_Point)
 btScalar				kVCF;			// Velocities correction factor (Baumgarte)
 btScalar				kDP;			// Damping coefficient [0,1]
 btScalar				kDG;			// Drag coefficient [0,+inf]
 btScalar				kLF;			// Lift coefficient [0,+inf]
 btScalar				kPR;			// Pressure coefficient [-inf,+inf]
 btScalar				kVC;			// Volume conversation coefficient [0,+inf]
 btScalar				kDF;			// Dynamic friction coefficient [0,1]
 btScalar				kMT;			// Pose matching coefficient [0,1]		
 btScalar				kCHR;			// Rigid contacts hardness [0,1]
 btScalar				kKHR;			// Kinetic contacts hardness [0,1]
 btScalar				kSHR;			// Soft contacts hardness [0,1]
 btScalar				kAHR;			// Anchors hardness [0,1]
 btScalar				kSRHR_CL;		// Soft vs rigid hardness [0,1] (cluster only)
 btScalar				kSKHR_CL;		// Soft vs kinetic hardness [0,1] (cluster only)
 btScalar				kSSHR_CL;		// Soft vs soft hardness [0,1] (cluster only)
 btScalar				kSR_SPLT_CL;	// Soft vs rigid impulse split [0,1] (cluster only)
 btScalar				kSK_SPLT_CL;	// Soft vs rigid impulse split [0,1] (cluster only)
 btScalar				kSS_SPLT_CL;	// Soft vs rigid impulse split [0,1] (cluster only)
 btScalar				maxvolume;		// Maximum volume ratio for pose
 btScalar				timescale;		// Time scale
 int						viterations;	// Velocities solver iterations
 int						piterations;	// Positions solver iterations
 int						diterations;	// Drift solver iterations
 int						citerations;	// Cluster solver iterations
 int						collisions;		// Collisions flags
 tVSolverArray			m_vsequence;	// Velocity solvers sequence
 tPSolverArray			m_psequence;	// Position solvers sequence
 tPSolverArray			m_dsequence;	// Drift solvers sequence		 
 
*/


MySoftBody::MySoftBody(btBroadphaseInterface* m_broadphase,
					   btCollisionDispatcher* m_dispatcher,
					   btVector3 gravity) {

    softBodyWI.m_broadphase = m_broadphase;
    softBodyWI.m_dispatcher = m_dispatcher;
	softBodyWI.m_gravity.setValue(gravity.x(), gravity.y(), gravity.z()); // too much gravity will penetrate ground	
	softBodyWI.air_density = 1.2;
	softBodyWI.water_density = 0;
	softBodyWI.water_offset = 0;
	softBodyWI.water_normal	= btVector3(0,0,0);
	softBodyWI.m_sparsesdf.Initialize();
	//softBodyWI.m_sparsesdf.GarbageCollect();

}

btSoftBody * MySoftBody::getSoftBody(){
	return psb;
}	


void MySoftBody::createRopeShape(btVector3 from, btVector3 len,
							int res, int fixed,
							int mass,
							ofxVec4f color) {
	
	bodyColor = color;
	
	psb = btSoftBodyHelpers::CreateRope(softBodyWI, 
										from,
										//to+len,
										from+len,
										res, fixed);
	psb->setTotalMass(mass);
	psb->setPose(true,true);	
	psb->m_materials[0]->m_kLST   =   1;
	psb->m_materials[0]->m_kAST   =   1;
	psb->m_materials[0]->m_kVST   =   1;
	psb->m_cfg.kMT = 0.002;
	psb->m_cfg.kDP = 0.05;
	psb->m_cfg.kKHR = 1; //*
	psb->m_cfg.kDF = 1;	//*
	
}

void MySoftBody::createEllipsoidShape(btVector3 center, btVector3 radius, int res) {
	
    psb = btSoftBodyHelpers::CreateEllipsoid(softBodyWI,
											 center,
											 radius,
											 res);
	psb->m_materials[0]->m_kLST   =   0.1;
	psb->m_cfg.kDF            =   1;
	psb->m_cfg.kDP            =   0.001;
	psb->m_cfg.kPR            =   2500;
	psb->setTotalMass(20,true);	
	psb->setPose(false, false);
}

void MySoftBody::createClothShape(ofxVec3f clothShape[4], 
								  int resolution, int fix) {
	
	/*
	 btSoftBodyWorldInfo& worldInfo,
	 const btVector3& corner00,
	 const btVector3& corner10,
	 const btVector3& corner01,
	 const btVector3& corner11,
	 int resx,
	 int resy,
	 int fixeds,
	 bool gendiags	 
	 */
	psb = btSoftBodyHelpers::CreatePatch(softBodyWI,
										 btVector3(clothShape[0].x, clothShape[0].y, clothShape[0].z),
										 btVector3(clothShape[1].x, clothShape[1].y, clothShape[1].z),
										 btVector3(clothShape[2].x, clothShape[2].y, clothShape[2].z),
										 btVector3(clothShape[3].x, clothShape[3].y, clothShape[3].z),
										 resolution,
										 resolution,
										 fix,
										 false);
	
	psb->getCollisionShape()->setMargin(0.5);
	btSoftBody::Material* pm = psb->appendMaterial();
	pm->m_kLST		=	0.04;
//	pm->m_kAST		=	0.9;
//	pm->m_kVST		=	0.9;	
	psb->generateBendingConstraints(1,pm);
	psb->setTotalMass(150);
	
//	psb->m_cfg.kMT = 0.2;
	psb->m_cfg.kDP = 0.01;
//	psb->m_cfg.kKHR = 0.6; //*

	

//	psb->m_cfg.collisions   =   btSoftBody::fCollision::CL_SS+btSoftBody::fCollision::CL_RS;	
	psb->m_cfg.aeromodel	=	btSoftBody::eAeroModel::V_TwoSided;

	psb->generateClusters(1024);
	
	psb->setTotalMass(100);
	
//	btTransform		trs;
//	trs.setIdentity();
//	psb->transform(trs);
//	psb->addForce(btVector3(0,2,0),0);
	
}

void MySoftBody::render() {

	btSoftBody::tNodeArray& nodes(psb->m_nodes);
	btSoftBody::tLinkArray& links(psb->m_links);
	btSoftBody::tFaceArray& faces(psb->m_faces);

	glColor4f(0.0, 0.0, 0.0, 1.0);
	
	for(int j=0;j<nodes.size();++j) {
		ofxPoint(nodes[j].m_x.getX(), nodes[j].m_x.getY(), nodes[j].m_x.getZ());
//		ofxSphere(nodes[j].m_x.getX(), nodes[j].m_x.getY(), nodes[j].m_x.getZ(), 0.1);
	}

//	int num = 0;
//	for(int j=0;j<links.size();++j) {
//		btSoftBody::Node*   node_0=links[j].m_n[0];
//		btSoftBody::Node*   node_1=links[j].m_n[1];
////		ofxLine(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(), 
////			  node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ());
//		
//		ofxVec3f node0Vec;
//		node0Vec.x = node_0->m_x.getX();
//		node0Vec.y = node_0->m_x.getY();
//		node0Vec.z = node_0->m_x.getZ();
//		ofxVec3f node1Vec;
//		node1Vec.x = node_1->m_x.getX();
//		node1Vec.y = node_1->m_x.getY();
//		node1Vec.z = node_1->m_x.getZ();
//
//		float distance = node0Vec.distance(node1Vec);
//		for (float i = 0; i < distance; i+=ofMap(j, 0, links.size(), 0.01, 1)) {
//			ofxVec3f interp = node0Vec.getInterpolated(node1Vec, ofMap(i, 0, distance, 0, 1));
//
//			ofPushMatrix();
//			ofTranslate(interp.x, interp.y, interp.z);
//			//ofRotate(90, 1, 0, 0);
//			ofCircle(0,0, (links.size()-j));
//			ofPopMatrix();					
//
//			num++;
//		}		
//	
////	//		if (j < 5) {
////	//			ofxLine(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(), 
////	//					node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ());
////	//		}else {
////			float distance = node0Vec.distance(node1Vec);
////			//for (float i = 0; i < distance; i+=0.1) {
////			for (float i = 0; i < distance; i+=ofMap(j, 0, links.size(), 0.01, 1)) {
////				ofxVec3f interp = node0Vec.getInterpolated(node1Vec, ofMap(i, 0, distance, 0, 1));
////				
////				if (j == links.size()-1) {
////					//ofxSphere(interp.x, interp.y, interp.z, j*0.1);
////					ofxSphere(interp.x, interp.y, interp.z, 0.5);
////					ballPos = ofxVec3f(interp.x, interp.y, interp.z);
////				}else {
////					ofPushMatrix();
////					ofTranslate(interp.x, interp.y, interp.z);
////					//ofRotate(90, 1, 0, 0);
////					ofCircle(0,0, j*0.03);
////					ofPopMatrix();					
////				}
////				
////				num++;
////	//			}
////			}
//
//	}
//	//cout << num << endl;
//
//	for(int j=0;j<faces.size();++j) {
//		btSoftBody::Node*   node_0=faces[j].m_n[0];
//		btSoftBody::Node*   node_1=faces[j].m_n[1];
//		btSoftBody::Node*   node_2=faces[j].m_n[2];
//		ofxTriangleShape(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ(),
//				  node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ(),
//				  node_2->m_x.getX(), node_2->m_x.getY(), node_2->m_x.getZ());
//
//
//	}
//
	
}
