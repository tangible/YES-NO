#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

//template <class T>
//class Base
//{
//    typedef std::pair<T, T> pair; // typename isn't needed here, this isn't dependent
//};

template <class T>
class Inheritor : public Base<T>
{
public: // public again
    typedef typename Base<T>::pair pair; // get the Base's pair type, typedef it
	
    pair *p; // ah, easy to use
};

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

};

#endif
