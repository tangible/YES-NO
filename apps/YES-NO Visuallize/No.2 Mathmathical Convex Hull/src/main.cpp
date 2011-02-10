#include "ofMain.h"
#include "App.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1440,900, OF_FULLSCREEN);
	ofRunApp(new App());

}
