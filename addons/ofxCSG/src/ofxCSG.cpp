
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/2008/09/constructive-solid-geometry-program.html
//
// Modified for OF by Karl D.D. Willis, 2011


#include "ofxCSG.h"

#include "Face.h"
#include "VertexSet.h"


ofxCSG::ofxCSG() {
	setupComplete = false;
}


ofxCSG::~ofxCSG() {
	ofLog(OF_LOG_VERBOSE, "Deleting object 1");
	if(m_pObject1 != NULL) delete m_pObject1;
	ofLog(OF_LOG_VERBOSE, "Deleting object 2");
	if(m_pObject2 != NULL)  delete m_pObject2;
}

void ofxCSG::setup(ofxCSGSolid * solid1, ofxCSGSolid * solid2) {
	ofLog(OF_LOG_VERBOSE, "Construction Object3D from Solid");
	
	if(setupComplete) {
		delete m_pObject1;
		delete m_pObject2;
	}

	//representation to apply boolean operations
	m_pObject1 = new Object3D(solid1);
	m_pObject2 = new Object3D(solid2);
	
	ofLog(OF_LOG_VERBOSE, "Object1 - Split against Object2");
	//split the faces so that none of them intercepts each other
	m_pObject1->splitFaces(m_pObject2);
	ofLog(OF_LOG_VERBOSE, "Object2 - Split against Object1");
	m_pObject2->splitFaces(m_pObject1);
	
	ofLog(OF_LOG_VERBOSE, "Object1 - Classify faces of Object2");
	
	//classify faces as being inside or outside the other solid
	m_pObject1->classifyFaces(*m_pObject2);
	
	ofLog(OF_LOG_VERBOSE, "Object2 - Classify faces of Object1");
	
	m_pObject2->classifyFaces(*m_pObject1);
	
	setupComplete = true;
}


ofxCSGSolid * ofxCSG::getUnion() {
	if(setupComplete) {
		return composeSolid(Face::OUTSIDE, Face::SAME, Face::OUTSIDE);
	} else {
		return NULL;
	}
}

ofxCSGSolid * ofxCSG::getIntersection() {
	if(setupComplete) {	
		return composeSolid(Face::INSIDE, Face::SAME, Face::INSIDE);
	} else {
		return NULL;
	}
}

ofxCSGSolid * ofxCSG::getDifference() {
	if(setupComplete) {	
		m_pObject2->invertInsideFaces();
		ofxCSGSolid * result = composeSolid(Face::OUTSIDE, Face::OPPOSITE, Face::INSIDE);
		m_pObject2->invertInsideFaces();
		return result;
	} else {
		return NULL;
	}
}

ofxCSGSolid * ofxCSG::composeSolid(int faceStatus1, int faceStatus2, int faceStatus3) 
{
//	VertexSet * vertices = new VertexSet();
//	IntSet * indices = new IntSet();
//	ColorSet * colors = new ColorSet();
	
	VertexSet vertices;
	IntSet indices;
	ColorSet colors;	

	//group the elements of the two solids whose faces fit with the desired status  
	groupObjectComponents(*m_pObject1, vertices, indices, colors, faceStatus1, faceStatus2);
	groupObjectComponents(*m_pObject2, vertices, indices, colors, faceStatus3, faceStatus3);
	
	//VectorSet * vectors = new VectorSet();
	vector<ofPoint> vectors;
	
	for(int i = 0; i < vertices.GetNumVertices(); i++) {
		Vertex * pVertex = vertices.GetVertex(i);
		mlVector3D pos = pVertex->getPosition();
		
		vectors.push_back(ofPoint(pos.x, pos.y, pos.z));
	}

	vector<ofColor> cols;
	for(int i = 0; i < colors.GetSize(); i++) {
		gxColor c = colors.GetColor(i);
		ofColor ofc;
		ofc.r = c.red;
		ofc.g = c.green;
		ofc.b = c.blue;
		ofc.a = c.alpha;
		cols.push_back(ofc);
	}
	
	
	ofxCSGSolid *result = new ofxCSGSolid();
	result->setup(vectors, indices.m_pInts, cols);

	return result;
}

void ofxCSG::groupObjectComponents(Object3D & object, VertexSet & vertices, IntSet & indices, ColorSet & colors, int faceStatus1, int faceStatus2)
{
	//for each face..
	for(int i=0;i<object.getNumFaces();i++)
	{
		Face & face = *(object.getFace(i));
		//if the face status fits with the desired status...
		if(face.getStatus()==faceStatus1 || face.getStatus()==faceStatus2)
		{
			//adds the face elements into the arrays 

			VertexPointerSet faceVerts;
			faceVerts.add(face.v1);
			faceVerts.add(face.v2);
			faceVerts.add(face.v3);

			for(int j=0;j<faceVerts.length();j++)
			{
				if(vertices.contains(faceVerts[j]))
				{
					indices.AddInt(vertices.indexOf(faceVerts[j]));
				}
				else
				{
					indices.AddInt(vertices.length());
					vertices.AddVertex(*faceVerts[j]);
					colors.AddColor(faceVerts[j]->getColor());
				}
			}
		}
	}
}
