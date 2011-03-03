/*
 *  ofxVCGLib.h
 *
 *  Created by Makira on 11/03/03.
 *  Copyright 2011 yesMAYBEno. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "BunnyMesh.h"

#include "vcglib/vcg/simplex/vertex/base.h"
#include "vcglib/vcg/simplex/face/base.h"
#include "vcglib/vcg/simplex/face/topology.h"
#include "vcglib/vcg/complex/trimesh/base.h"
#include "vcglib/vcg/complex/trimesh/update/bounding.h"
#include "vcglib/vcg/complex/trimesh/update/topology.h"
#include "vcglib/vcg/complex/trimesh/update/normal.h"
#include "vcglib/vcg/complex/trimesh/update/flag.h"
#include "vcglib/vcg/complex/trimesh/create/ball_pivoting.h"
//#include "vcglib/vcg/space/box3.h"
//#include "vcglib/vcg/space/normal_extrapolation.h"

#include "cmesh.h"

using namespace vcg;

struct Hinge { 
	int v0, v1, v2;   //v0, v1 represent the Hinge, v2 the other vertex in the face
	//this Hinge belongs to
	int face;        //corresponding face
	Point3f center;  //center of the sphere touching the face
	int count;   //test delay touch Hinges.  
	
	//the loops in the front are mantained as a double linked list
	list<Hinge>::iterator next;            
	list<Hinge>::iterator previous;
    
	Hinge() {}
	Hinge(int _v0, int _v1, int _v2, int _face, Point3f &_center): 
	v0(_v0), v1(_v1), v2(_v2), 
	face(_face), center(_center), count(0) {
		assert(v0 != v1 && v1 != v2 && v0 != v2);
	}
};


class ofxVCGLib {

public:
	
	void reconstructFaceBunny(); // for bunny mesh test :)
	
	// if radius ==0 an autoguess for the ball pivoting radius is attempted 
	// otherwise the passed value (in absolute mesh units) is used.	
	void reconstructFacePointCloud(vector<float> point_cloud, 
								   float _radius = 0, 
								   float _clustering = 0.2,
								   float _angle = M_PI/2);
	
	int addFace();
	void addFace(int a, int b, int c);
	bool seed(bool outside = true, int start = -1);
	unsigned int getInSphere(vcg::Point3f &p, 
							 float distance, 
							 vector<int> &results,
							 vector<float> &dists);
	bool findSphere(Point3f &p0, Point3f &p1, Point3f &p2, Point3f &center);
	void cluster(int v);
	bool pivot(Hinge &Hinge, int &candidate, Point3f &end_pivot, std::vector<int> &targets);
	float angle(Point3f p, Point3f q, Point3f &axis);
	void killHinge(std::list<Hinge>::iterator e);
	list<Hinge>::iterator touches(int v, std::list<Hinge>::iterator e);
	bool checkHinge(int v0, int v1);
	void detach(int v);
	void moveBack(list<Hinge>::iterator e);
	void trovamiunnome(list<Hinge>::iterator e);
	bool glue(list<Hinge>::iterator a, list<Hinge>::iterator b);
	list<Hinge>::iterator newHinge(Hinge e);
	
	vector<float> getVertices();
	vector<int> getFaceIndices();
	int getFaceNum();
	
	CMesh &m;	
    float radius;  //default 1 (not meaningful
    float mindist; //minimum distance between points in the mesh (% of radius)   
    float crease;  // -0.5 
	Box3f box;
	typedef GridStaticPtr< CMesh::VertexType, CMesh::ScalarType > StaticGrid;		
	StaticGrid grid;
	
    /* front Hinges of the mesh: 
	 to expand the front we get the first Hinge
	 if an Hinge cannot create a new triangle it is marked dead and moved
	 to the end of the list
	 the new Hinges are inserted to the back (before dead_begin) */
    list<Hinge> front;  	
    list<Hinge> deads;	
    vector<int> nb; //number of fronts a vertex is into,
					//this is used for the Visited and Border flags
					//but adding topology may not be needed anymode	
	
};