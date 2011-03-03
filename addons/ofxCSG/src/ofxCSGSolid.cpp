
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/2008/09/constructive-solid-geometry-program.html
//
// Modified for OF by Karl D.D. Willis, 2011


#include "ofxCSGSolid.h"

#include "GX_Color.h"

ofxCSGSolid::ofxCSGSolid() {
	setupComplete = false;
}

ofxCSGSolid::~ofxCSGSolid() {
}

//---------------------------------------SETUP-----------------------------------//

/**
 * Setup using solid data. An exception may occur in the case of abnormal arrays 
 * (indices making references to inexistent vertices, there are less colors 
 * than vertices...)
 * 
 * @param vertices array of points defining the solid vertices
 * @param indices array of indices for a array of vertices
 * @param colors array of colors defining the vertices colors 
 */
void ofxCSGSolid::setup(vector<ofPoint> &verts, vector<int> &inds, vector<ofColor> &cols){
	
	vertices.Clear();
	colors.Clear();
	indices.Clear();
	
	//cout << vertices.GetSize() << endl;
	
	if(inds.size() > 0) {
		for(int i=0; i<verts.size(); i++) {
			// Should add cloning to vectorset, and other 2 sets
			
			vertices.AddVector( mlVector3D(verts[i].x, verts[i].y, verts[i].z) );
			colors.AddColor( gxColor(cols[i].r, cols[i].g, cols[i].b) );
			//this->indices.AddInt(indices[i]);
		}
		
		for(int i=0; i<inds.size(); i++) {
			indices.AddInt(inds[i]);
		}
		setupComplete = true;
	}
}


/**
 * Loads a coordinates file, setting vertices and indices 
 * 
 * @param solidFile file used to create the solid
 * @param color solid color
 */
void ofxCSGSolid::setup(const std::string & filename, int red, int green, int blue) {
	
	std::ifstream modelFile;
	std::string fullFilePath = ofToDataPath(filename);
	
	modelFile.open(fullFilePath.c_str());
	
	int nNumVertices = 0;
	modelFile >> nNumVertices;
	
	for(int i = 0; i < nNumVertices; i++) {
		// Read in a vector.
		int nVertexID = 0;
		
		mlVector3D vPosition;
		
		modelFile >> nVertexID;
		
		modelFile >> vPosition.x;
		modelFile >> vPosition.y;
		modelFile >> vPosition.z;
		
		vPosition = mlVectorScale(vPosition, mlVector3D(0.5f, 0.5f, 0.5f));
		
		vertices.AddVector(vPosition);
	}
	
	// Now load up the indices.
	
	int nNumTriangles = 0;
	modelFile >> nNumTriangles;
	
	for(int i = 0; i < nNumTriangles; i++) {
		int nTriangleID = 0;
		
		int nTriIndex1 = 0;
		int nTriIndex2 = 0;
		int nTriIndex3 = 0;
		
		modelFile >> nTriangleID;
		
		modelFile >> nTriIndex1;
		modelFile >> nTriIndex2;
		modelFile >> nTriIndex3;
		
		indices.AddInt(nTriIndex1);
		indices.AddInt(nTriIndex2);
		indices.AddInt(nTriIndex3);
	}
	
	modelFile.close();
	
	for(int i = 0; i < nNumVertices; i++) {
		gxColor col(red, green, blue);	
		colors.AddColor(col);
	}
	
	setupComplete = true;
}


//---------------------------------------GETS-----------------------------------//

void ofxCSGSolid::getVertices(vector<ofPoint> &verts) {

	for(int i = 0; i < vertices.length(); i++) {
		mlVector3D mv = vertices.GetVector(i);
		verts.push_back(ofPoint(mv.x, mv.y, mv.z));
	}

}

void ofxCSGSolid::getIndices(vector<int> &inds) {

	for(int i = 0; i < indices.length(); i++) {
		inds.push_back(indices[i]);
	}

}

void ofxCSGSolid::getColors(vector<ofColor> &cols) {

	for(int i = 0; i < colors.GetSize(); i++) {
		gxColor c = colors.GetColor(i);
		ofColor ofc;
		ofc.r = c.red;
		ofc.g = c.green;
		ofc.b = c.blue;
		ofc.a = c.alpha;
		cols.push_back(ofc);
	}
	
}

VectorSet * ofxCSGSolid::getVertices()
{
	// This thing makes a fresh copy and hands the requestor the copy.
	
	VectorSet * newVertices = new VectorSet();
	
	for(int i = 0; i < vertices.length(); i++)
	{
		newVertices->AddVector(vertices[i]);
	}
	
	return newVertices;
}

IntSet * ofxCSGSolid::getIndices()
{
	IntSet * newIndices = new IntSet();
	
	for(int i = 0; i < indices.length(); i++)
	{
		newIndices->AddInt(indices[i]);
	}
	
	return newIndices;
}

ColorSet * ofxCSGSolid::getColors()
{
	ColorSet * newColors = new ColorSet();
	
	for(int i = 0; i < colors.length(); i++)
	{
		newColors->AddColor(colors[i]);
	}
	
	return newColors;
}

bool ofxCSGSolid::isEmpty()
{
	if(indices.length() <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//---------------------------------------SETS-----------------------------------//

/**
 * Sets the solid color (all the vertices with the same color)
 * 
 * @param color solid color
 */
void ofxCSGSolid::setColor(const int red, const int green, const int blue) {
	
	gxColor color(red, green, blue);
	
	for(int i=0; i < vertices.length(); i++) {
		colors.SetColor(i, color);
	}
}

//-------------------------GEOMETRICAL_TRANSFORMATIONS-------------------------//

void ofxCSGSolid::translate(const float x, const float y, const float z) {
	
	mlVector3D t(x, y, z);
	
	for(int i = 0; i < vertices.GetSize(); i++)
	{
		mlVector3D v = vertices.GetVector(i);

		v = v + t;

		vertices.SetVector(i, v);
	}
}

void ofxCSGSolid::rotate(float x, float y, float z, float angle){
	
	mlVector3D a(x, y, z);
	mlVector3D b(0, 0, 0);
	
	for(int i = 0; i < vertices.GetSize(); i++) {
		mlVector3D v = vertices.GetVector(i);
		
		v = mlVectorRotate(v, a, b, angle);
		
		vertices.SetVector(i, v);
	}
}

void ofxCSGSolid::rotate(const ofPoint &a, const ofPoint &b, float angle) {

	mlVector3D ma(a.x, a.y, a.z);
	mlVector3D mb(b.x, b.y, b.z);
	
	for(int i = 0; i < vertices.GetSize(); i++) {
		mlVector3D v = vertices.GetVector(i);

		v = mlVectorRotate(v, ma, mb, angle);

		vertices.SetVector(i, v);
	}
}

void ofxCSGSolid::scale(float x, float y, float z) {
	
	mlVector3D s(x, y, z);
	
	for(int i = 0; i < vertices.GetSize(); i++)	{
		mlVector3D v = vertices.GetVector(i);

		v = mlVectorScale(v, s);

		vertices.SetVector(i,v);
	}
}

void ofxCSGSolid::draw() {
	drawFill();
	drawOutline();
}

void ofxCSGSolid::drawFill() {
//	glPushMatrix();
//	
//	mlMatrix4x4 mMatrix = mlMatrix4x4(m_correctionalTransform.GetMatrix());
//	mlMatrix4x4 * pMatrix = &mMatrix;
//	mlFloat * pafMatrix = reinterpret_cast<mlFloat*>(pMatrix);
//
//	glMultMatrixf(pafMatrix);
//
//	glEnable (GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset (1., 1.);

//	glLineWidth(1);

	int nNumTriangles = indices.GetSize() / 3;
	
	glBegin(GL_TRIANGLES);

	for(int i = 0; i < nNumTriangles; i++)
	{
		int nIndex1 = indices.GetInt(i * 3 + 0);
		int nIndex2 = indices.GetInt(i * 3 + 1);
		int nIndex3 = indices.GetInt(i * 3 + 2);

		mlVector3D vP1 = vertices.GetVector(nIndex1);
		mlVector3D vP2 = vertices.GetVector(nIndex2);
		mlVector3D vP3 = vertices.GetVector(nIndex3);

//		float fScale = 40.0f;
//		vP1 *= fScale;
//		vP2 *= fScale;
//		vP3 *= fScale;
//
//		mlVector3D vTranslation(50,20,50);
//		vP1 += vTranslation;
//		vP2 += vTranslation;
//		vP3 += vTranslation;

		gxColor col1 = colors.GetColor(nIndex1);
		gxColor col2 = colors.GetColor(nIndex2);
		gxColor col3 = colors.GetColor(nIndex3);

		glColor4ub(col1.red, col1.green, col1.blue, 255);
		glVertex3d(vP1.x, vP1.y, vP1.z);

		glColor4ub(col2.red, col2.green, col2.blue, 255);
		glVertex3d(vP2.x, vP2.y, vP2.z);

		glColor4ub(col3.red, col3.green, col3.blue, 255);
		glVertex3d(vP3.x, vP3.y, vP3.z);
	}

	glEnd();
}
	
void ofxCSGSolid::drawOutline() {	

	int nNumTriangles = indices.GetSize() / 3;
	
	glBegin(GL_LINES);

	for(int i = 0; i < nNumTriangles; i++) {
		int nIndex1 = indices.GetInt(i * 3 + 0);
		int nIndex2 = indices.GetInt(i * 3 + 1);
		int nIndex3 = indices.GetInt(i * 3 + 2);

		mlVector3D vP1 = vertices.GetVector(nIndex1);
		mlVector3D vP2 = vertices.GetVector(nIndex2);
		mlVector3D vP3 = vertices.GetVector(nIndex3);

		glColor4ub(0, 0, 0, 255);

		glVertex3d(vP1.x, vP1.y, vP1.z);
		glVertex3d(vP2.x, vP2.y, vP2.z);

		glVertex3d(vP2.x, vP2.y, vP2.z);
		glVertex3d(vP3.x, vP3.y, vP3.z);

		glVertex3d(vP3.x, vP3.y, vP3.z);
		glVertex3d(vP1.x, vP1.y, vP1.z);
	}

	glEnd();
}

/**
 * Gets the solid mean
 * 
 * @return point representing the mean
 */
ofPoint ofxCSGSolid::getMean()
{
	ofPoint mean;
	for(int i=0;i<vertices.length();i++)
	{
		mean.x += vertices[i].x;
		mean.y += vertices[i].y;
		mean.z += vertices[i].z;
	}
	mean.x /= vertices.length();
	mean.y /= vertices.length();
	mean.z /= vertices.length();
	
	return mean;
}


