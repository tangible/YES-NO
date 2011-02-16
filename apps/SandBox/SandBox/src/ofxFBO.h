#pragma once

#include "ofGraphics.h"

class ofxFBO {
protected:
	int width, height;
	GLuint fboId, depthId;
	ofImage* attached;
public:
	ofxFBO() :
			fboId(0),
			depthId(0),
			attached(NULL) {
	}
	void setup(int width, int height) {
		this->width = width;
		this->height = height;
		glGenFramebuffersEXT(1, &fboId);
		glGenRenderbuffersEXT(1, &depthId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthId);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthId);
		swapOut();
	}
	~ofxFBO() {
		if(fboId != 0) {
			glDeleteFramebuffersEXT(1, &fboId);
			glDeleteRenderbuffersEXT(1, &depthId);
		}
	}
	void setTarget(ofImage& target) {
		swapIn();

		// detach old texture
		if(attached != NULL) {
			ofTextureData texData = attached->getTextureReference().texData;
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, texData.textureTarget, 0, 0);
		}

		// attach new texture
		attached = &target;
		ofTextureData texData = attached->getTextureReference().texData;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, texData.textureTarget, texData.textureID, 0); // attach

		swapOut();
	}
	void swapIn() {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthId);
	}
	void swapOut() {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}
	void begin() {
		swapIn();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void end() {
		swapOut();
	}
};
