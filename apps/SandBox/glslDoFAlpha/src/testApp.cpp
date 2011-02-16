#include "testApp.h"



#define DEFAULT_DISTANCE 9.0f
#define DEFAULT_RANGE 2.0f

#define NUM_SHADERS 6
#define NUM_FBOS 5

void DrawQuad(int numMultiTex)
{
	glBegin(GL_QUADS);{
		if(numMultiTex == 0){
			glTexCoord2f(0.0f, 0.0f);
		}
		else{
			glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
			glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
			if(numMultiTex == 3){
				glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
			}
		}
		glVertex2d(-1, -1);
		
		if(numMultiTex == 0){
			glTexCoord2f(1.0f, 0.0f);
		}
		else{
			glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
			glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
			if(numMultiTex == 3){
				glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
			}
		}
		glVertex2d(1, -1);
		
		if(numMultiTex == 0){
			glTexCoord2f( 1.0f, 1.0f);
		}
		else{
			glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
			glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
			if(numMultiTex == 3){
				glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
			}
		}
		glVertex2d(1, 1);
		
		if(numMultiTex == 0){
			glTexCoord2f(0.0f, 1.0f);
		}
		else {
			glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
			glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
			if(numMultiTex == 3){
				glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
			}
		}
		glVertex2d(-1, 1);
		
	}glEnd();
}

void DrawCube(int open, float texsize)
{
	glBegin(GL_QUADS);
	if (!open) {
		/* Front Face */
		glTexCoord2f(texsize, texsize);
		glVertex3f(-1.0, -1.0,  1.0);
		glTexCoord2f(0.0f, texsize);
		glVertex3f( 1.0, -1.0,  1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f( 1.0,  1.0,  1.0);
		glTexCoord2f(texsize, 0.0f);
		glVertex3f(-1.0,  1.0,  1.0);
	}
	/* Back Face */
	glTexCoord2f(0.0f, texsize);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0,  1.0, -1.0);
	glTexCoord2f(texsize, 0.0f);
	glVertex3f( 1.0,  1.0, -1.0);
	glTexCoord2f(texsize, texsize);
	glVertex3f( 1.0, -1.0, -1.0);
	if (!open) {
		/* Top Face */
		glTexCoord2f(texsize, 0.0f);
		glVertex3f(-1.0,  1.0, -1.0);
		glTexCoord2f(texsize, texsize);
		glVertex3f(-1.0,  1.0,  1.0);
		glTexCoord2f(0.0f, texsize);
		glVertex3f( 1.0,  1.0,  1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f( 1.0,  1.0, -1.0);
	}
	/* Bottom Face */
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(texsize, 0.0f);
	glVertex3f( 1.0, -1.0, -1.0);
	glTexCoord2f(texsize, texsize);
	glVertex3f( 1.0, -1.0,  1.0);
	glTexCoord2f(0.0f, texsize);
	glVertex3f(-1.0, -1.0,  1.0);
	// Right face
	glTexCoord2f(0.0f, texsize);
	glVertex3f( 1.0, -1.0, -1.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( 1.0,  1.0, -1.0);
	glTexCoord2f(texsize, 0.0f);
	glVertex3f( 1.0,  1.0,  1.0);
	glTexCoord2f(texsize, texsize);
	glVertex3f( 1.0, -1.0,  1.0);
	// Left Face
	glTexCoord2f(texsize, texsize);
	glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0.0f, texsize);
	glVertex3f(-1.0, -1.0,  1.0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0,  1.0,  1.0);
	glTexCoord2f(texsize, 0.0f);
	glVertex3f(-1.0,  1.0, -1.0);
	glEnd();
}

//--------------------------------------------------------------
void testApp::setup(){

	ofSetupScreen();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	ofDisableArbTex();
	ofSetVerticalSync(true);
	//ofHideCursor();

	//init FBOs
	fbos = (ofxFBOMSTexture**) calloc(sizeof(ofxFBOMSTexture*), NUM_FBOS);
	float w,h;
	for(int i = 0; i < NUM_FBOS; i++){
		fbos[i] = new ofxFBOMSTexture();
		if(i == 0 || i == 1){
			w = ofGetWidth(); 
			h = ofGetHeight();
		}
		else {
			w = ofGetWidth()/2;
			h = ofGetHeight()/2;
		}
		printf("allocating FBO %d \n", i);
		fbos[i]->allocate(w, h, true);
	}
	
	//init Shaders
	char *files[NUM_SHADERS] = {"shaders/first", "shaders/second", "shaders/third", "shaders/fourth", "shaders/fifth", "shaders/renderblur"};
	shaders = (ofShader**) calloc(sizeof(ofShader*), NUM_SHADERS);
	for(int i = 0; i < NUM_SHADERS; i++){
		shaders[i] = new ofShader();
		shaders[i]->loadShader(files[i]);
	}
	
	shaders[1]->setShaderActive(true);
	shaders[1]->setUniform("Tex0", 0);
	shaders[1]->setUniform("Tex1", 1);
	shaders[1]->setShaderActive(false);
	
	shaders[2]->setShaderActive(true);
	shaders[2]->setUniform("Width", ofGetWidth()*2);
	
	shaders[3]->setShaderActive(true);
	shaders[3]->setUniform("Height", ofGetHeight()*2);
		
	shaders[4]->setShaderActive(true);
	shaders[4]->setUniform("Tex0", 0);
	shaders[4]->setUniform("Tex1", 1);
	shaders[4]->setUniform("Tex2", 2);
	shaders[4]->setShaderActive(false);

	
	focalDistance = DEFAULT_DISTANCE;
	focalRange = DEFAULT_RANGE;

	background.allocate(512, 512, OF_IMAGE_COLOR_ALPHA);
	brick.allocate(512, 512, OF_IMAGE_COLOR_ALPHA);
	
	background.loadImage("textures/background.png");
	brick.loadImage("textures/brick.png");

	width = ofGetWidth();
	height = ofGetHeight();
	
	rot = 0;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0, 0, 0);
	//depthofield
	/**
	 * FBO description:
	 * fbos[0] is the full resolution scene with alpha
	 * fobs[1] is the scene with blur info stored in alpha
	 * fbos[1] is the downsampled version of fbos[0] for blurring
	 * fbos[2] x blurred
	 * fbos[3] x&y blurred
	 */
	fbos[0]->swapIn();{
		
		ofEnableAlphaBlending();
		
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		drawParticles();
		
		ofDisableAlphaBlending();
	}fbos[0]->swapOut();
	

	//Render Depth Map
	fbos[1]->swapIn();{
		focalDistance = 1.0*mouseX/ofGetWidth() * 20.0f;
		focalRange = 1.0*mouseY/ofGetHeight() * 10.0f;
		
		shaders[0]->setShaderActive(true);
		shaders[0]->setUniform("focalDistance", focalDistance);
		shaders[0]->setUniform("focalRange", focalRange);	
		
		drawParticles();
		
		shaders[0]->setShaderActive(false);
	}fbos[1]->swapOut();
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	//downsample
	fbos[2]->swapIn();{	
		glViewport(0, 0, width/2, height/2);
		
		glActiveTexture(GL_TEXTURE0);	
		fbos[0]->bindAsTexture();
		
		glActiveTexture(GL_TEXTURE1);
		fbos[1]->bindAsTexture();
		
		shaders[1]->setShaderActive(true);
		
		DrawQuad(2);

		shaders[1]->setShaderActive(false);
	
		
	}fbos[2]->swapOut();
	
	//blur x
	fbos[3]->swapIn();{
		
		glActiveTexture(GL_TEXTURE0);		
		fbos[2]->bindAsTexture();
				
		shaders[2]->setShaderActive(true);	
		
		DrawQuad(0);	
	
		shaders[2]->setShaderActive(false);
		
	}fbos[3]->swapOut();
	
	//blur y
	fbos[4]->swapIn();{
		
		glActiveTexture(GL_TEXTURE0);
		fbos[3]->bindAsTexture();

		shaders[3]->setShaderActive(true);	
	
		DrawQuad(0);
		
		shaders[3]->setShaderActive(false);
		
	}fbos[4]->swapOut();
	
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//full resolution
	glActiveTexture(GL_TEXTURE0);
	fbos[0]->bindAsTexture();
	
	//blurred
	glActiveTexture(GL_TEXTURE1);
	fbos[4]->bindAsTexture();

	//depth
	glActiveTexture(GL_TEXTURE2);
	fbos[1]->bindAsTexture();
	
	shaders[4]->setShaderActive(true);
	
	glViewport(0, 0, width, height);

	DrawQuad(3);
	
	shaders[4]->setShaderActive(false);

}

void testApp::drawParticles()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)ofGetWidth()/(GLfloat)ofGetHeight(), 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	/* room */
	background.getTextureReference().bind();
	glPushMatrix();{
		glTranslatef(0.0f, 6.0f, -10.0f);
		glScalef(150.0f, 150.0f, 150.0f);
		DrawCube(1, 1.0);
	}glPopMatrix();
	
	brick.getTextureReference().bind();	
	glPushMatrix();{
		glScalef(2, 2, 2);
		glTranslatef(0.0f, 0.0f, -5.0f);
		//wiggly!
		glRotatef(90 + sin(rot) * 15, 0, 1, 0);
		rot += .005; 
		DrawCube(0, 1.0);
	}glPopMatrix();
	//draw some stuff
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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



