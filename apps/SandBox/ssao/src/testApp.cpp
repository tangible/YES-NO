#include "testApp.h"

static GLuint attachTexture(GLint format, GLsizei width, GLsizei height, GLenum attachment)
{
	// テクスチャオブジェクトの作成
	GLuint texname;
	glGenTextures(1, &texname);
	
	// テクスチャの結合
	glBindTexture(GL_TEXTURE_2D, texname);
	
	// テクスチャの割り当て
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
				 (format == GL_DEPTH_COMPONENT) ? GL_DEPTH_COMPONENT : GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	// テクスチャパラメータ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// フレームバッファオブジェクトに結合
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, texname, 0);
	
	return texname;
}


static const GLfloat kd[] = { 0.5f, 0.5f, 0.1f, 0.6f };     // 拡散反射係数 (alpha > 0 にする)
static const GLfloat ks[] = { 0.2f, 0.2f, 0.2f, 0.0f };     // 鏡面反射係数 (alpha = 0 にする)
static const GLfloat kshi = 40.0f;                          // 輝き係数
static const GLfloat kr[] = { 0.2f, 0.2f, 0.2f, 0.0f };     // 映り込みの反射率 (alpha = 0 にする)
static const GLfloat keta = 0.67f;                          // 屈折率の比
static const GLfloat kbgd = 0.1f;                           // 背景との仮想的な距離
static GLuint fbo;
static const GLenum bufs[] = {             
	GL_COLOR_ATTACHMENT0_EXT,              
	GL_COLOR_ATTACHMENT1_EXT,              
	GL_COLOR_ATTACHMENT2_EXT,              
	GL_COLOR_ATTACHMENT3_EXT,              
};
#define BUFNUM (sizeof bufs / sizeof bufs[0])
#define TEXNUM (BUFNUM + 3)
static GLuint tex[TEXNUM];
static GLint unit;           
#define MAXSAMPLES 256                      
#define SAMPLERADIUS 0.1f                   
static GLfloat pointbuf[MAXSAMPLES][4];

static const GLfloat lpos[] = { 0.0f, 0.0f, 10.0f, 1.0f };  
static const GLfloat lcol[] = { 0.5f, 0.5f, 0.5f, 1.0f };   
static const GLfloat lamb[] = { 0.8f, 0.8f, 0.8f, 0.0f };   
static GLdouble ex = 0.0, ey = 0.0, ez = -1.5;


//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetDataPathRoot("../Resources/");
//	ofBackground(255, 255, 255);
//	ofEnableAlphaBlending();
	ofDisableArbTex();
	
//	cam.setup(this, 700);
	
	pass1.setup("pass1");
	pass2.setup("pass2");
	
	numObj = 100;
	for (int i = 0; i < numObj; i++) {
		float x = ofRandom(ofGetWidth()/2-200, ofGetWidth()/2+200);
		float y = ofRandom(ofGetHeight()/2-200, ofGetHeight()/2+200);
		float z = ofRandom(-200, 200);
		ofxVec3f pos = ofxVec3f(x,y,z);
		float r = ofRandom(0.0, 255.0);
		float g = ofRandom(0.0, 255.0);
		float b = ofRandom(0.0, 255.0);
		float a = ofRandom(200.0, 255.0);
		ofxVec4f col = ofxVec4f(r,g,b,a);
		int size = ofRandom(10, 50);
		objPos.push_back(pos);
		objCol.push_back(col);
		objSize.push_back(size);
	}
	
	int BUFWIDTH = ofGetWidth();
	int BUFHEIGHT = ofGetHeight();
	
	// フレームバッファオブジェクトの作成
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	
	// 拡散反射光 D + 鏡面反射光 S
	tex[0] = attachTexture(GL_RGBA, BUFWIDTH, BUFHEIGHT, bufs[0]);
	
	// 環境光の反射光 A
	tex[1] = attachTexture(GL_RGBA, BUFWIDTH, BUFHEIGHT, bufs[1]);
	
	// 位置 P
	tex[2] = attachTexture(GL_RGBA32F_ARB, BUFWIDTH, BUFHEIGHT, bufs[2]);
	
	// 法線 N
	tex[3] = attachTexture(GL_RGBA32F_ARB, BUFWIDTH, BUFHEIGHT, bufs[3]);
	
	// 深度
	tex[4] = attachTexture(GL_DEPTH_COMPONENT, BUFWIDTH, BUFHEIGHT, GL_DEPTH_ATTACHMENT_EXT);
	
	// フレームバッファオブジェクトの結合を解除
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	// レンダリングターゲットをクリアする値
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// デフォルトのテクスチャに戻す
	glBindTexture(GL_TEXTURE_2D, 0);	
	
	// サンプル点の生成
	for (int i = 0; i < MAXSAMPLES; ++i) {
		float r = SAMPLERADIUS * (float)rand() / (float)RAND_MAX;
		float t = 6.2831853f * (float)rand() / ((float)RAND_MAX + 1.0f);
		float cp = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
		float sp = sqrt(1.0f - cp * cp);
		float ct = cos(t), st = sin(t);
		
		pointbuf[i][0] = r * sp * ct;
		pointbuf[i][1] = r * sp * st;
		pointbuf[i][2] = r * cp;
		pointbuf[i][3] = 0.0f;
	}	
	
}

//--------------------------------------------------------------
void testApp::update(){

	
}

//--------------------------------------------------------------
void testApp::draw(){

//	cam.draw();
//	ofxVec3f cPos = cam.getPosition();
	
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	glEnable(GL_DEPTH_TEST);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glDrawBuffers(BUFNUM, bufs);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	pass1.begin();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(ex, ey, ez);	
//	glTranslated(cPos.x, cPos.y, cPos.z);	
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lcol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lcol);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lamb);	
	
	drawScene();
	
	pass1.end();	
	
	
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	glDisable(GL_DEPTH_TEST);		
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);	
	pass2.begin();
	pass2.setUniform4fv("reflection", (float*)kr, 1);
	pass2.setUniform1f("refraction", keta);
	pass2.setUniform1f("bgdistance", kbgd);
	
	GLint unitname[TEXNUM];
	for (int i = 0; i < TEXNUM; ++i) {
		unitname[i] = i;
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tex[i]);
	}
	pass2.setUniform1iv("unit", unitname, TEXNUM);
	pass2.setUniform4fv("point", pointbuf[0], MAXSAMPLES);
	
	static const GLfloat quad[][2] = {
		{ -1.0f, -1.0f },
		{  1.0f, -1.0f },
		{  1.0f,  1.0f },
		{ -1.0f,  1.0f },
	};
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof quad / sizeof quad[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);	
	pass2.end();
	
	glutSwapBuffers();
	
	ofxSphere(ofGetWidth()/2-100, ofGetHeight()/2, 0, 100);
}

//--------------------------------------------------------------
void testApp::drawScene() {
	
	ofPushMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, kshi);	
	
	for (int i = 0; i < numObj; i++) {
		ofxVec3f pos = objPos[i];
		ofxVec4f col = objCol[i];
		int size = objSize[i];
		ofSetColor(col.x, col.y, col.z);
		ofxSphere(pos.x, pos.y, pos.z, size);
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){}
//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

