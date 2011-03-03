
// Author: Greg Santucci, 2008 and 2009
// This is part of a port of the CSG project
// originally written in java by Danilo Balby
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef LINE3D_H
#define LINE3D_H

#include "ML_Vector.h"
#include "Face.h"

class Line3D
{
public:

	mlVector3D point;
	mlVector3D direction;
	
	static double TOL;

	Line3D();

	Line3D(Face * face1, Face * face2);
	Line3D(const mlVector3D & direction, const mlVector3D & point);
	
	mlVector3D getPoint();
	mlVector3D getDirection();
	void setPoint(const mlVector3D & point);
	void setDirection(const mlVector3D & direction);
		
	double computePointToPointDistance(const mlVector3D & otherPoint);
	
	mlVector3D computeLineIntersection(Line3D * otherLine);
	mlVector3D computePlaneIntersection(const mlVector3D & normal, const mlVector3D & planePoint, bool & bResult);

	void perturbDirection();
};

#endif // LINE_H
