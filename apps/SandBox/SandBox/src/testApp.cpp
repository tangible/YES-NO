#include "testApp.h"

void testApp::setup() {
	w = ofGetWidth();
	h = ofGetHeight();

	blendShader.setup("blendShader", "blendShader");

	fbo.setup(w, h);
	left.allocate(w, h, OF_IMAGE_COLOR_ALPHA);
	right.allocate(w, h, OF_IMAGE_COLOR_ALPHA);

	isMousePressed = false;
}

void testApp::update() {
}

void testApp::mousePressed(int x, int y, int button) {
	isMousePressed = true;
}

void testApp::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}

void testApp::texRect(int w, int h) {
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(0, h);
	glVertex2f(0, h);
	glTexCoord2f(w, 0);
	glVertex2f(w, 0);
	glTexCoord2f(w, h);
	glVertex2f(w, h);
	glEnd();
}

void testApp::runShader(ofxFBO& fbo, ofxShader& shader) {
	fbo.begin();
	shader.begin();
	texRect(w, h);
	shader.end();
	fbo.end();
}

void testApp::setTexture(char* name, ofxShader& shader, ofImage& img, int location) {
	ofTextureData texData = img.getTextureReference().getTextureData();
	glActiveTexture(GL_TEXTURE0 + location);
	glEnable(texData.textureTarget);
	glBindTexture(texData.textureTarget, texData.textureID);
	glDisable(texData.textureTarget);
	shader.setUniform(name, location);
}

void testApp::draw() {
	int r = 200;

	ofBackground(0, 0, 0);

	fbo.setTarget(left);
	fbo.begin();
	glColor4f(.5, 0, 0, 1);
	ofCircle(w / 3, h / 2, r);
	fbo.end();

	fbo.setTarget(right);
	fbo.begin();
	glColor4f(0, .5, 0, 1);
	ofCircle(2 * w / 3, h / 2, r);
	fbo.end();

	glColor4f(1, 1, 1, 1);

	if(isMousePressed) {
		blendShader.begin();
		blendShader.setTexture("top", left, 0);
		blendShader.setTexture("base", right, 1);
		texRect(w, h);
		blendShader.end();
	} else {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		left.draw(0, 0);
		right.draw(0, 0);
		glDisable(GL_BLEND);
	}
}
