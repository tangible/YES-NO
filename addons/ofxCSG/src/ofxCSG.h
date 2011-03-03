
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/2008/09/constructive-solid-geometry-program.html
//
// Modified for OF by Karl D.D. Willis, 2011


#ifndef OFXCSG_H
#define OFXCSG_H

#include "Object3D.h"
#include "ofxCSGSolid.h"

#include "ofTypes.h"


class ofxCSG {

public:

	ofxCSG();
	virtual ~ofxCSG();
	
	void setup(ofxCSGSolid * solid1, ofxCSGSolid * solid2);
	
	ofxCSGSolid * getUnion();
	ofxCSGSolid * getIntersection();
	ofxCSGSolid * getDifference();

private:
	
	bool setupComplete;
	
	Object3D * m_pObject1;
	Object3D * m_pObject2;
	
	ofxCSGSolid * composeSolid(int faceStatus1, int faceStatus2, int faceStatus3);
	
	void groupObjectComponents(Object3D & pObject, VertexSet & vertices, IntSet & indices, ColorSet & colors, int faceStatus1, int faceStatus2);
};

#endif 