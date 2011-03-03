
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/2008/09/constructive-solid-geometry-program.html
//
// Modified for OF by Karl D.D. Willis, 2011


#ifndef OFXCSG_SOLID_H
#define OFXCSG_SOLID_H

#include "IntSet.h"
#include "VectorSet.h"
#include "ColorSet.h"

#include "ofTypes.h"

class ofxCSGSolid {

public:

	// These are left in the original data format for now...
	IntSet indices;
	VectorSet vertices;
	ColorSet colors;
	
	ofxCSGSolid();
	virtual ~ofxCSGSolid();
	
	void setup(vector<ofPoint> &verts, vector<int> &inds, vector<ofColor> &cols);
	void setup(const std::string & filename, int red, int green, int blue);
	
	// Get the data in an of friendly format
	void getVertices(vector<ofPoint> &verts);
	void getIndices(vector<int> &inds);
	void getColors(vector<ofColor> &cols);
	
	// Return pointers in the original data format
	VectorSet * getVertices();
	IntSet * getIndices();
	ColorSet * getColors();

	bool isEmpty();

	void setColor(int red, int green, int blue);

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z, float angle);
	void rotate(const ofPoint &a, const ofPoint &b, float angle);
	void scale(float x, float y, float z);

	void draw();
	void drawFill();
	void drawOutline();

	ofPoint getMean();
	
private: 
	
	bool setupComplete;

};

#endif 
