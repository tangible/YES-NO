#include "testApp.h"
#include "ofxSimpleGuiToo.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	cam.setup(this, 760);
	
//	vcglib = new ofxVCGLib();
	reconstruct(300.0, 0.2, M_PI/2);

	gui.addTitle("VCG Setting");
	gui.addSlider("radius", radius, 0, 1000);
	gui.addSlider("clustering", cludtering, 0.2, 1.0);
	gui.addSlider("angle", angle, 0.0, 10.0);
	gui.loadFromXML();
	gui.show();	
	radius = 0;
	cludtering = 0.2;
	angle = 2.0;
}

void testApp::reconstruct(float radius, float clustering, float _angle) {
	
	int arrSize = 8*3;
	vector<float> input;
	float distAvg = 0.0;
	
	for (int i = 0; i < arrSize; i++) {
		float p = verts[i];
		input.push_back(p);
	}
	
	vcglib.reconstructFacePointCloud(input, radius, clustering, _angle);
//	vcgpiv = new ofxVCGPivot<CMesh>(vcglib.m, radius, clustering, _angle);
//	vcglib.reconstructFaceBunny();
//	vcgpiv = new ofxVCGPivot<CMesh>(vcglib.m, vcglib.getRadius(), vcglib.getClustering(), vcglib.getAngle());	
	
	float colFactor = 1.0/vcglib.m.face.size();
	for (int i = 0; i < vcglib.m.face.size(); i++) {
		colp.setColorScale(1.0);
		colp.setColorRadius(1.0);
		colp.setColorAngle(colFactor*i);
		colp.update();
		ofColor col = colp.getColor();
		cols.push_back(col);
	}	
	
}

//--------------------------------------------------------------
void testApp::update(){

	//reconstruct(radius, cludtering, M_PI/angle);
	
}

//--------------------------------------------------------------
void testApp::draw(){

	glEnable(GL_DEPTH_TEST);
	
	cam.draw();
	
	int test = vcglib.m.face.size();
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
//	ofScale(150, 150, 150);
	
	ofSetColor(0, 0, 0);
	for (int i = 0; i < vcglib.m.vert.size(); i++) {
		ofxPoint(vcglib.m.vert[i].P()[0], vcglib.m.vert[i].P()[1], vcglib.m.vert[i].P()[2]);
	}
	
	for (int i = 0; i < vcglib.m.face.size(); i++) {
//		ofSetColor(cols[i].r, cols[i].g, cols[i].b);
		CFace &face = vcglib.m.face[i];	
		CVertex *v[3];		
		v[0] = face.V(0);
		v[1] = face.V(1);
		v[2] = face.V(2);		
		float x0 = v[0]->P()[0];
		float y0 = v[0]->P()[1];
		float z0 = v[0]->P()[2];		
		float x1 = v[1]->P()[0];
		float y1 = v[1]->P()[1];
		float z1 = v[1]->P()[2];		
		float x2 = v[2]->P()[0];
		float y2 = v[2]->P()[1];
		float z2 = v[2]->P()[2];		
//		float x0 = ->P()[0];
//		float y0 = vcglib.m.face[i].V(0)->P()[1];
//		float z0 = vcglib.m.face[i].V(0)->P()[2];		
//		float x1 = vcglib.m.face[i].V(1)->P()[0];
//		float y1 = vcglib.m.face[i].V(1)->P()[1];
//		float z1 = vcglib.m.face[i].V(1)->P()[2];		
//		float x2 = vcglib.m.face[i].V(2)->P()[0];
//		float y2 = vcglib.m.face[i].V(2)->P()[1];
//		float z2 = vcglib.m.face[i].V(2)->P()[2];		
		ofxVec3f v0(x0, y0, z0);
		ofxVec3f v1(x1, y1, z1);
		ofxVec3f v2(x2, y2, z2);
		ofxTriangleShape(v0, v1, v2);
	}
	
	ofPopMatrix();
	
	ofSetupScreen();
	gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	int i = vcglib.pivot->addFace();
	cout << ofToString(i) << endl;
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

