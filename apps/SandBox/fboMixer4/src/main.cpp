
// include all of the header files

#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

	// create an opengl window
    ofAppGlutWindow window;
	
	// for multisampling:
	window.setGlutDisplayString("rgba double depth samples>=4");
	
	// setup the window at a given size in normal (non-fullcreen mode )
	// use OF_FULLSCREEN if you want to change the window mode
	ofSetupOpenGL(&window, 1024,768, OF_WINDOW);			

	// create an instance of the testApp and run it as our main application
	ofRunApp( new testApp());
}
