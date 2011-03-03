#ifndef CLOTH_MESH_H
#define CLOTH_MESH_H

#include "ofMain.h"

#include "vcglib/vcg/simplex/vertex/base.h"
#include "vcglib/vcg/simplex/face/base.h"
#include "vcglib/vcg/simplex/face/pos.h"
#include "vcglib/vcg/simplex/face/topology.h"
#include "vcglib/vcg/complex/trimesh/base.h"
#include "vcglib/vcg/complex/trimesh/update/normal.h"

#include <vector>

using namespace vcg;

class CVertex;
class CEdge;
class CFace;

struct CUsedTypes : public UsedTypes<Use<CVertex> ::AsVertexType, Use<CFace> ::AsFaceType, Use<CEdge> ::AsEdgeType>{};

class CEdge {
public:
	CVertex *v[2];         
	CFace *f;
	bool operator<(const CEdge& t) const {
		if(v[0] < t.v[0]) return true;
		if(v[0] > t.v[0]) return false;
		return v[1] < t.v[1];
	}
	bool operator==(const CEdge& t) const {
		return v[0] == t.v[0] && v[1] == t.v[1];
	}
};

class CVertex: public
Vertex<CUsedTypes,
		vertex::Coord3f, 
		vertex::Normal3f, 
		vertex::BitFlags, 
		vertex::Mark,
		vertex::VFAdj, 
		vertex::Qualityf> {
public:     
	float color;
};

class CFace: public Face <CUsedTypes, 
							face::VertexRef, 
							face::Normal3f,
							face::BitFlags, 
							face::VFAdj, face::FFAdj > {};

class CMesh: public tri::TriMesh< vector<CVertex>, vector<CFace> > {};

#endif
