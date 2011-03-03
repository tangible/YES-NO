#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#include "ofxCSG.h"


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
	
	
		// SCREEN DATA
		int prev_x, prev_y;
		int halfWidth, halfHeight;
	
		// ORBIT VIEW
		float view_rotx; 
		float view_roty;
		float view_rotz;
		float zoom;
		void resetRotation();
		void rotate(int x, int y);		
	
		// CSG 
		ofxCSG model;
		ofxCSGSolid solid1, solid2;	
	
};

#endif
