/*
 *  Quat.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 23/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 *	Source from - http://3dengine.org/Quaternions
 *
 */

#ifndef _H_QUAT
#define _H_QUAT

class Quat
{
	
public :
	
	Quat();
	~Quat();
	void set( float x1, float y1, float z1, float degrees );
	void gl_matrix( double *m );
	Quat operator* ( const Quat &b ) const;
	
private :
	
    float w,x,y,z;
	
};

#endif _H_QUAT