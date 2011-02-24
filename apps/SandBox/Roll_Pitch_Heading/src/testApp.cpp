#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	incDir	= 1;
	
	roll	= 0;
	pitch	= -27;
	heading	= 45;
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{

}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	Quat qr;	// quat roll.
	Quat qp;	// quat pitch.
	Quat qh;	// quat heading.
	Quat qt;	// quat total.
	
	// The order IS IMPORTANT. Apply roll first, then pitch, then heading.
	qr.set( 0, 0, 1, roll);
	qp.set( 1, 0, 0, pitch);
	qh.set( 0, 1, 0, heading);
	
	qt = qr * qp * qh;
	double *m = new double[ 16 ] ;
	qt.gl_matrix( m );
	
	glPushMatrix();
	glTranslatef( ofGetWidth() * 0.5, ofGetHeight() * 0.5, 0 );
	glMultMatrixd( m );
	glScalef( 200, 200, 200 );
	
	drawCube();
	
	glPopMatrix();
	
	ofSetColor( 0x000000 );
	ofDrawBitmapString
	(
		"fps :: " + ofToString(ofGetFrameRate(), 2) + "\n\n"
		"incDir  :: press i to change :: " + ofToString( incDir ) + "\n\n"
		"roll    :: press 1 to change :: " + ofToString( roll ) + "\n\n"
		"pitch   :: press 2 to change :: " + ofToString( pitch ) + "\n\n"
		"heading :: press 3 to change :: " + ofToString( heading ) + "\n\n",
		20,
		20
	);
}

void testApp :: drawCube()
{
	GLfloat box[] = 
	{
		// FRONT
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		// BACK
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		// LEFT
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		// RIGHT
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		// TOP
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		// BOTTOM
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
	};
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	
	glVertexPointer(3, GL_FLOAT, 0, box);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	float a = 1.0f;		// alpha
	
	// FRONT AND BACK
	glColor4f(1.0f, 0.0f, 0.0f, a );
	glNormal3f(0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	
	// LEFT AND RIGHT
	glColor4f(0.0f, 1.0f, 0.0f, a );
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
	
	// TOP AND BOTTOM
	glColor4f(0.0f, 0.0f, 1.0f, a );
	glNormal3f(0.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{
	if( key == 'i' )
	{
		incDir *= -1;
	}
	
	float inc = 3;
	
	if( key == '1' )
	{
		roll += inc * incDir;
	}
	
	if( key == '2' )
	{
		pitch += inc * incDir;
	}
	
	if( key == '3' )
	{
		heading += inc * incDir;
	}
	
	if( key == ' ' )
	{
		roll	= 0;
		pitch	= 0;
		heading	= 0;
	}
}

void testApp :: keyReleased( int key )
{
	//
}

void testApp :: mouseMoved( int x, int y )
{

}

void testApp :: mouseDragged( int x, int y, int button )
{

}

void testApp :: mousePressed( int x, int y, int button )
{

}

void testApp :: mouseReleased( int x, int y, int button )
{

}

void testApp :: windowResized( int w, int h )
{

}

