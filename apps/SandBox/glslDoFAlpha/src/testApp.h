/**
 * Depth of Field in openFrameworks example
 * shaders adapted from:
 * http://encelo.netsons.org/programming/opengl
 *
 */
 
#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofShader.h"
#include "ofxFBOMSTexture.h"
#include "ofxVectorMath.h"


class testApp : public ofBaseApp{

  public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
  
  protected:
	void drawParticles();
	
	ofxFBOMSTexture** fbos;
	ofShader** shaders;
		
	float focalDistance;
	float focalRange;

	ofImage brick, background;

	float width, height;

	float rot;
};

#endif
